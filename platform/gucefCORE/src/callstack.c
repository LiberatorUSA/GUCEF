/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_MT_MUTEX_H
#include "gucefMT_mutex.h"
#define GUCEF_MT_MUTEX_H
#endif /* GUCEF_MT_MUTEX_H ? */

#include "callstack.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define STACK_RESIZE_AMOUNT     256

#ifdef GUCEF_MSWIN_BUILD
  #define EOL                   "\r\n"
#else
  #define EOL                   "\n"
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SCallStack
{
        const char** file;
        int* linenr;
        UInt64* entryTickCount;
        UInt32 items;
        UInt32 stacksize;
        UInt32 threadid;
};
typedef struct SCallStack TCallStack;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static TCallStack* _stacks = NULL;
static UInt32 stackcount = 0;
static char* logFilename = NULL;
static FILE* logFile = NULL;
static struct SMutex* mutex = NULL;
static UInt32 logStack = 0;
static UInt32 isInitialized = 0;
static UInt32 logInCvsFormatBool = 1;
static TStackPushCallback pushCallback = NULL;
static TStackPopCallback popCallback = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static void
Log( const char* logtype ,
     UInt32 threadID     ,
     Int32 stackheight   ,
     const char* file    ,
     int line            ,
     UInt32 ticksSpent   )
{
    if ( logStack != 1 )
    {
        if ( NULL == logFile )
        {
            /* lazy initialization of the default output file */
            if ( NULL == logFilename )
            {
                logFilename = (char*) malloc( 14 );
                memcpy( logFilename, "Callstack.txt", 14 );
                logFile = fopen( logFilename, "ab" );
            }
            else
            {
                logFile = fopen( logFilename, "ab" );
            }
        }

        if ( logInCvsFormatBool == 0 )
        {
            if ( ticksSpent > 0 )
            {
                fprintf( logFile, "Thread %d: %s: %d: %s(%d) (%d ms)%s", threadID, logtype, stackheight, file, line, ticksSpent, EOL );
            }
            else
            {
                fprintf( logFile, "Thread %d: %s: %d: %s(%d)%s", threadID, logtype, stackheight, file, line, EOL );
            }
        }
        else
        {
            if ( ticksSpent > 0 )
            {
                fprintf( logFile, "%d,%s,%d,%s,%d,%d%s", threadID, logtype, stackheight, file, line, ticksSpent, EOL );
            }
            else
            {
                fprintf( logFile, "%d,%s,%d,%s,%d%s", threadID, logtype, stackheight, file, line, EOL );
            }
        }
        fflush( logFile );
    }
}

/*-------------------------------------------------------------------------*/

static void
Push( TCallStack* stack ,
      const char* file  ,
      int line          )
{
    /* if the stack heap is full we will enlarge it */
    if ( stack->items == stack->stacksize )
    {
        stack->stacksize += STACK_RESIZE_AMOUNT;
        stack->file = (const char**)realloc( (char**)stack->file, stack->stacksize*sizeof(const char*) );
        stack->linenr = (int*)realloc( stack->linenr, stack->stacksize*sizeof(int) );
        stack->entryTickCount = (UInt64*)realloc( stack->entryTickCount, stack->stacksize*sizeof(UInt64) );
    }

    /* record the new call on the stack */
    stack->file[ stack->items ] = file;
    stack->linenr[ stack->items ] = line;
    stack->entryTickCount[ stack->items ] = PrecisionTickCount();
    ++stack->items;

    Log( "PUSH", stack->threadid, stack->items, file, line, 0 );

    if ( pushCallback != NULL )
    {
        pushCallback( file            ,
                      line            ,
                      stack->threadid ,
                      stack->items    );
    }
}

/*-------------------------------------------------------------------------*/

static void
Pop( TCallStack* stack )
{
    Int32 itemCount;
    if ( stack->items > 0 )
    {
        UInt32 ticksSpent;

        --stack->items;
        itemCount = stack->items;
        ticksSpent = (UInt32) (PrecisionTickCount() - stack->entryTickCount[ itemCount ]);

        Log( " POP", stack->threadid, itemCount+1, stack->file[ itemCount ], stack->linenr[ itemCount ], ticksSpent );

        if ( popCallback != NULL )
        {
            popCallback( stack->file[ itemCount ]   ,
                         stack->linenr[ itemCount ] ,
                         stack->threadid            ,
                         stack->items               ,
                         ticksSpent                 );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_UtilityCodeBegin( const char* file ,
                        int line         )
{
    if ( isInitialized == 1 )
    {
        UInt32 i, threadid;

        MutexLock( mutex );

        /* try to find a stack for the caller thread */
        threadid = GetCurrentTaskID();
        for ( i=0; i<stackcount; ++i )
        {
                if ( _stacks[ i ].threadid == threadid )
                {
                        Push( &_stacks[ i ] ,
                              file          ,
                              line          );
                        MutexUnlock( mutex );
                        return;
                }
        }

        /* no stack found for the caller thread,.. make one */
        _stacks = (TCallStack*) realloc( _stacks, (stackcount+1)*sizeof(TCallStack) );
        _stacks[ stackcount ].threadid = threadid;
        _stacks[ stackcount ].items = 0;
        _stacks[ stackcount ].stacksize = 0;
        _stacks[ stackcount ].file = NULL;
        _stacks[ stackcount ].linenr = NULL;
        _stacks[ stackcount ].entryTickCount = NULL;
        Push( &_stacks[ stackcount ] ,
              file                   ,
              line                   );
        stackcount++;

        MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_UtilityCodeEnd( void )
{
    if ( isInitialized == 1 )
    {
        UInt32 i, threadid;

        MutexLock( mutex );

        threadid = GetCurrentTaskID();
        for ( i=0; i<stackcount; ++i )
        {
            if ( _stacks[ i ].threadid == threadid )
            {
                Pop( &_stacks[ i ] );
                break;
            }
        }

        MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

static void
PrintCallstack( FILE* dest )
{
    UInt32 i, n;
    for ( i=0; i<stackcount; ++i )
    {
        fprintf( dest, "------------------------------%s", EOL );
        fprintf( dest, "Callstack for thread %d:%s%s", _stacks[ i ].threadid, EOL, EOL );
        fprintf( dest, "stack size = %d%s", _stacks[ i ].items, EOL );
        fprintf( dest, "------------------------------%s%s", EOL, EOL );
        if ( _stacks[ i ].items )
        {
            for ( n=0; n<_stacks[ i ].items; ++n )
            {
                    fprintf( dest, "%d: %s(%d)%s", n+1, _stacks[ i ].file[ n ], _stacks[ i ].linenr[ n ], EOL );
            }
            fprintf( dest, "%s%s------------------------------%s%s", EOL, EOL, EOL, EOL );
        }
        else
        {
            fprintf( dest, ">>> no items on stack <<<%s%s", EOL, EOL );
            fprintf( dest, "------------------------------%s%s", EOL, EOL );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_PrintCallstack( void )
{
    if ( isInitialized == 1 )
    {
        MutexLock( mutex );

        PrintCallstack( stdout );

        MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_DumpCallstack( const char* filename )
{
    if ( isInitialized == 1 )
    {
        FILE* fptr = NULL;

        MutexLock( mutex );

        fptr = fopen( filename, "wb" );
        if ( fptr )
        {
                PrintCallstack( fptr );
                fclose( fptr );
        }

        MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_InitCallstackUtility( void )
{
    if ( isInitialized == 0 )
    {
        mutex = MutexCreate();
        isInitialized = 1;
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_ShutdowntCallstackUtility( void )
{
    if ( isInitialized == 1 )
    {
        MutexLock( mutex );
        if ( ( logFile != stdout ) && ( logFile != NULL ) )
        {
            fclose( logFile );
        }
        free( logFilename );
        logFilename = NULL;

        isInitialized = 0;
        MutexDestroy( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackLogging( const UInt32 logStackBool )
{
    if ( isInitialized == 1 )
    {
        MutexLock( mutex );
        logStack = logStackBool;
        if ( logStack == 1 )
        {
            if ( ( logFilename != NULL ) &&
                 ( logFile == NULL )      )
            {
                logFile = fopen( logFilename, "ab" );
            }
            else
            {
                MutexUnlock( mutex );
                GUCEF_LogStackToStdOut();
                return;
            }
        }
        else
        {
            if ( ( logFile != NULL )  &&
                 ( logFile != stdout ) )
            {
                fclose( logFile );
                logFile = NULL;
            }
        }
        MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_LogStackToStdOut( void )
{
    if ( isInitialized == 1 )
    {
        MutexLock( mutex );
        if ( ( logFile != NULL )  &&
             ( logFile != stdout ) )
        {
            fclose( logFile );
        }
        logFile = stdout;
        free( logFilename );
        logFilename = NULL;
        MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackLoggingInCvsFormat( const UInt32 logAsCvsBool )
{
    MutexLock( mutex );
    logInCvsFormatBool = logAsCvsBool;
    MutexUnlock( mutex );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackPushCallback( TStackPushCallback cBack )
{
    MutexLock( mutex );
    pushCallback = cBack;
    MutexUnlock( mutex );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackPopCallback( TStackPopCallback cBack )
{
    MutexLock( mutex );
    popCallback = cBack;
    MutexUnlock( mutex );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_LogStackTo( const char* filename )
{
    if ( isInitialized == 1 )
    {
        UInt32 strLen;

        MutexLock( mutex );
        if ( ( logFile != stdout ) && ( logFile != NULL ) )
        {
            fclose( logFile );
        }
        free( logFilename );
        logFilename = NULL;

        strLen = (UInt32) strlen( filename )+1;
        logFilename = (char*) malloc( strLen );
        memcpy( logFilename, filename, strLen );
        logFile = fopen( logFilename, "ab" );

        MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/
