/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "callstack.h"

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_MT_MUTEX_H
#include "gucefMT_mutex.h"
#define GUCEF_MT_MUTEX_H
#endif /* GUCEF_MT_MUTEX_H ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h" 
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

/*
 *  We specifically do NOT want to redirect memory management here
 */
#ifndef GUCEF_DYNNEWOFF_H
#include "gucef_dynnewoff.h"
#define GUCEF_DYNNEWOFF_H
#endif /* GUCEF_DYNNEWOFF_H ? */

#undef GUCEF_USE_CALLSTACK_TRACING
#undef GUCEF_USE_CALLSTACK_PLATFORM_TRACING

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace MLF {
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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static MT::CMutex g_stackTraceMutex;
static TCallStack* _stacks = NULL;
static UInt32 stackcount = 0;
static char* logFilename = NULL;
static FILE* logFile = NULL;
static UInt32 logStack = 0;
static UInt32 isInitialized = 0;
static UInt32 logInCvsFormatBool = 1;
static TStackPushCallback pushCallback = NULL;
static TStackPopCallback popCallback = NULL;

static MT::SMutex* mutex = NULL;

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
     Int32 line          ,
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
      Int32 line        )
{
    /* if the stack heap is full we will enlarge it */
    if ( stack->items == stack->reservedStacksize )
    {
        stack->reservedStacksize += STACK_RESIZE_AMOUNT;
        stack->file = (const char**)realloc( (char**)stack->file, stack->reservedStacksize*sizeof(const char*) );
        stack->linenr = (int*)realloc( stack->linenr, stack->reservedStacksize*sizeof(Int32) );
        stack->entryTickCount = (UInt64*)realloc( stack->entryTickCount, stack->reservedStacksize*sizeof(UInt64) );
        stack->staticsAreCopied = 0;
    }

    /* record the new call on the stack */
    stack->file[ stack->items ] = file;
    stack->linenr[ stack->items ] = line;
    stack->entryTickCount[ stack->items ] = MT::PrecisionTickCount();
    ++stack->items;

    //Log( "PUSH", stack->threadid, stack->items, file, line, 0 );

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
        ticksSpent = (UInt32) (MT::PrecisionTickCount() - stack->entryTickCount[ itemCount ]);

        //Log( " POP", stack->threadid, itemCount+1, stack->file[ itemCount ], stack->linenr[ itemCount ], ticksSpent );

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
MEMMAN_CallstackScopeBegin( const char* file ,
                            Int32 line       )
{
    UInt32 i=0, threadid=0;

    MT::CScopeMutex lock( g_stackTraceMutex );

    /* try to find a stack for the caller thread */
    threadid = MT::GetCurrentTaskID();
    for ( i=0; i<stackcount; ++i )
    {
            if ( _stacks[ i ].threadid == threadid )
            {
                    Push( &_stacks[ i ] ,
                            file          ,
                            line          );
                    return;
            }
    }

    /* no stack found for the caller thread,.. make one */
    TCallStack* newStacks = (TCallStack*) realloc( _stacks, (stackcount+1)*sizeof(TCallStack) );
    if ( GUCEF_NULL != newStacks )
    {
        _stacks = newStacks; 
        _stacks[ stackcount ].threadid = threadid;
        _stacks[ stackcount ].items = 0;
        _stacks[ stackcount ].reservedStacksize = 0;
        _stacks[ stackcount ].file = GUCEF_NULL;
        _stacks[ stackcount ].linenr = GUCEF_NULL;
        _stacks[ stackcount ].entryTickCount = GUCEF_NULL;
        Push( &_stacks[ stackcount ] ,
                file                   ,
                line                   );
        stackcount++;
    }
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_CallstackScopeEnd( void )
{
    UInt32 i=0, threadid=0;

    MT::CScopeMutex lock( g_stackTraceMutex );

    threadid = MT::GetCurrentTaskID();
    for ( i=0; i<stackcount; ++i )
    {
        if ( _stacks[ i ].threadid == threadid )
        {
            Pop( &_stacks[ i ] );
            break;
        }
    }
}

/*-------------------------------------------------------------------------*/

Int32
MEMMAN_GetCallstackForCurrentThread( TCallStack** outStack )
{
    UInt32 i=0, threadid=0;

    if ( GUCEF_NULL == outStack )
        return -1;

    MT::CScopeMutex lock( g_stackTraceMutex );

    threadid = MT::GetCurrentTaskID();
    for ( i=0; i<stackcount; ++i )
    {
        if ( _stacks[ i ].threadid == threadid )
        {
            *outStack = &_stacks[ i ];
            return 1;
        }
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

Int32
MEMMAN_GetCallstackCopyForCurrentThread( TCallStack** outStack  ,
                                         UInt32 alsoCopyStatics )
{
    MT::CScopeMutex lock( g_stackTraceMutex );
    
    TCallStack* scopeStack = GUCEF_NULL;
    Int32 result = MEMMAN_GetCallstackForCurrentThread( &scopeStack );
    if ( 1 == result )
    {
        TCallStack* stackCopy = (TCallStack*) malloc( sizeof( TCallStack ) );
        if ( GUCEF_NULL != stackCopy )
        {
            stackCopy->threadid = scopeStack->threadid;
            stackCopy->items = scopeStack->items;
            stackCopy->reservedStacksize = stackCopy->items;

            UInt64 blockSize = stackCopy->items * sizeof(UInt64);
            stackCopy->entryTickCount = (UInt64*) malloc( blockSize );
            if ( GUCEF_NULL == stackCopy->entryTickCount )
            {
                free( stackCopy );
                *outStack = GUCEF_NULL;
                return -2;
            }
            memcpy( stackCopy->entryTickCount, scopeStack->entryTickCount, blockSize );
            
            blockSize = stackCopy->items * sizeof( const char*);
            stackCopy->file = (const char**) malloc( blockSize );
            if ( GUCEF_NULL == stackCopy->file )
            {
                free( stackCopy->entryTickCount );
                free( stackCopy );
                *outStack = GUCEF_NULL;
                return -2;
            }
            if ( 0 == alsoCopyStatics )
            {
                memcpy( stackCopy->file, scopeStack->file, blockSize );
                stackCopy->staticsAreCopied = 0;
            }
            else
            {
                /* there are cases where we also have to copy statics like the __FILE__ referenced strings
                 * this is because if we are unloading modules the pointers would be invalided even though they are
                 * hardcoded. This is an additional performance penalty so dont use if you dont need module unload protection
                 */

                memset( stackCopy->file, 0, blockSize );
                for ( UInt32 s=0; s<stackCopy->items; ++s )
                {
                    size_t filePathLength = strlen( scopeStack->file[ s ] );
                    char* newFilePathBuffer = (char*) malloc( filePathLength+1 );
                    if ( GUCEF_NULL != newFilePathBuffer )
                    {
                        stackCopy->file[ s ] = newFilePathBuffer;
                        memcpy( (void*)stackCopy->file[ s ], scopeStack->file[ s ], filePathLength+1 );
                    }
                    else
                    {
                        for ( UInt32 s2=0; s2<s; ++s2 )
                        {
                            free( (void*) stackCopy->file[ s ] );
                        }

                        free( stackCopy->entryTickCount );
                        free( stackCopy );
                        *outStack = GUCEF_NULL;
                        return -2;
                    }
                }
                stackCopy->staticsAreCopied = 1;
            }

            blockSize = stackCopy->items * sizeof(Int32);
            stackCopy->linenr = (Int32*) malloc( blockSize );
            if ( GUCEF_NULL == stackCopy->linenr )
            {
                free( stackCopy->entryTickCount );
                free( stackCopy->file );
                free( stackCopy );
                *outStack = GUCEF_NULL;
                return -2;
            }
            memcpy( stackCopy->linenr, scopeStack->linenr, blockSize );

            *outStack = stackCopy;
            return 1;
        }
        *outStack = GUCEF_NULL;
        return -2;
    }
    return result;
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_FreeCallstackCopy( TCallStack* stackCopy )
{
    if ( GUCEF_NULL != stackCopy )
    {
        if ( 0 != stackCopy->staticsAreCopied )
        {
            for ( UInt32 s=0; s<stackCopy->items; ++s )
            {
                free( (void*) stackCopy->file[ s ] );
            }
        }
        free( stackCopy->file );
        free( stackCopy->linenr );
        free( stackCopy->entryTickCount );
        free( stackCopy );
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
        MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );

        PrintCallstack( stdout );

        MT::MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_DumpCallstack( const char* filename )
{
    if ( isInitialized == 1 )
    {
        FILE* fptr = NULL;

        MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );

        fptr = fopen( filename, "wb" );
        if ( fptr )
        {
                PrintCallstack( fptr );
                fclose( fptr );
        }

        MT::MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_ShutdowntCallstackUtility( void )
{
    if ( isInitialized == 1 )
    {
        MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );
        if ( ( logFile != stdout ) && ( logFile != NULL ) )
        {
            fclose( logFile );
        }
        free( logFilename );
        logFilename = NULL;

        isInitialized = 0;
        MT::MutexDestroy( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackLogging( const UInt32 logStackBool )
{
    if ( isInitialized == 1 )
    {
        MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );
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
                MT::MutexUnlock( mutex );
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
        MT::MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_LogStackToStdOut( void )
{
    if ( isInitialized == 1 )
    {
        MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );
        if ( ( logFile != NULL )  &&
             ( logFile != stdout ) )
        {
            fclose( logFile );
        }
        logFile = stdout;
        free( logFilename );
        logFilename = NULL;
        MT::MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackLoggingInCvsFormat( const UInt32 logAsCvsBool )
{
    MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );
    logInCvsFormatBool = logAsCvsBool;
    MT::MutexUnlock( mutex );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackPushCallback( TStackPushCallback cBack )
{
    MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );
    pushCallback = cBack;
    MT::MutexUnlock( mutex );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackPopCallback( TStackPopCallback cBack )
{
    MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );
    popCallback = cBack;
    MT::MutexUnlock( mutex );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_LogStackTo( const char* filename )
{
    if ( isInitialized == 1 )
    {
        UInt32 strLen;

        MT::MutexLock( mutex, GUCEF_MUTEX_INFINITE_TIMEOUT );
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

        MT::MutexUnlock( mutex );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace MLF */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/
