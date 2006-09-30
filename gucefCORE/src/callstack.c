/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "dvoswrap.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#include "callstack.h"

/*-------------------------------------------------------------------------*/

#ifdef DEBUG_MODE

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

#ifdef MSWIN_BUILD
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
static FILE* log = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static void
Log( const char* logtype ,
     Int32 stackheight   ,
     const char* file    ,
     int line            )
{
   /*     log = fopen( "GUCEFCallstackHist.txt", "ab" );
        if ( !log ) return;
        
        fprintf( log, "%s: %d: %s(%d)%s", logtype, stackheight, file, line, EOL );
        
        fclose( log );  */
}     

/*-------------------------------------------------------------------------*/

static void
Push( TCallStack* stack ,
      const char* file  ,
      int line          )
{
        if ( stack->items == stack->stacksize )
        {
                stack->stacksize+=STACK_RESIZE_AMOUNT; 
                stack->file = (const char**)realloc( (char**)stack->file, stack->stacksize*sizeof(const char*) );         
                stack->linenr = (int*)realloc( stack->linenr, stack->stacksize*sizeof(int) );
        }
        
        stack->file[ stack->items ] = file;
        stack->linenr[ stack->items ] = line;
        ++stack->items;
        
        Log( "PUSH", stack->items, file, line ); 
}          

/*-------------------------------------------------------------------------*/

void 
GUCEF_UtilityCodeBegin( const char* file ,
                        int line         )
{
        UInt32 i, threadid = GetCurrentTaskID();
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
                
        _stacks = (TCallStack*) realloc( _stacks, (stackcount+1)*sizeof(TCallStack) );
        _stacks[ stackcount ].threadid = threadid;
        _stacks[ stackcount ].items = 0;
        _stacks[ stackcount ].stacksize = 0;
        _stacks[ stackcount ].file = NULL;
        _stacks[ stackcount ].linenr = NULL;        
        Push( &_stacks[ stackcount ] ,
              file                   ,
              line                   );
        stackcount++;              
}                        

/*-------------------------------------------------------------------------*/
                       
void                        
GUCEF_UtilityCodeEnd( void )
{
        UInt32 i, threadid = GetCurrentTaskID();
        for ( i=0; i<stackcount; ++i )
        {
                if ( _stacks[ i ].threadid == threadid )
                {
                        if ( _stacks[ i ].items )
                        {
                                --_stacks[ i ].items;
                                Log( " POP", _stacks[ i ].items+1, _stacks[ i ].file[ _stacks[ i ].items ], _stacks[ i ].linenr[ _stacks[ i ].items ] );                                                
                                return;
                        }                                                        
                }
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
        PrintCallstack( stdout );         
}

/*-------------------------------------------------------------------------*/

void
GUCEF_DumpCallstack( const char* filename )
{
        FILE* fptr = fopen( filename, "wb" );
        if ( fptr )
        {
                PrintCallstack( fptr );
                fclose( fptr );  
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

#endif /* DEBUG_MODE ? */
