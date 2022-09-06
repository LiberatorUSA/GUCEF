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
#include <assert.h>
#include <map>

#include "gucefMLF_callstack.h"

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

#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

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

namespace GUCEF {
namespace MLF {

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_HIDDEN StackInventory
{
    public:

    StackInventory( void );
    ~StackInventory();

    static StackInventory* Instance( void );
    static void Deinstance( void );
    static bool IsInitialized( void );

    void SetStackPushCallback( TStackPushCallback cBack );
    void SetStackPopCallback( TStackPopCallback cBack );

    void CallstackScopeBegin( const char* file , Int32 line );
    void CallstackScopeEnd( void );
    
    Int32 GetCallstackForCurrentThread( TCallStack** outStack );
    Int32 GetCallstackCopyForCurrentThread( TCallStack** outStack, bool alsoCopyStatics );
    void FreeCallstackCopy( TCallStack* stackCopy );
    
    void Log( const char* logtype ,
              UInt32 threadID     ,
              Int32 stackheight   ,
              const char* file    ,
              Int32 line          ,
              UInt32 ticksSpent   );

    void PrintCallstack( FILE* dest );    
    void SetIsStackLoggingEnabled( bool isEnabled );
    void SetIsStackLoggingInCsvFormat( bool isInCsv );
    void SetLogFilename( const char* filename );
    void SetStackLogOutputToStdOut( void );
    void CloseLogFile( void );

    private:

    class GUCEF_HIDDEN StackTraceInfo
    {
        public:

        TCallStack m_callstack;

        static void Push( TCallStack* stack ,
                          const char* file  ,
                          Int32 line        );

        static void Pop( TCallStack* stack );

        void PrintCallstack( FILE* dest );
        void CallstackScopeBegin( const char* file , Int32 line );
        void CallstackScopeEnd( void );
        Int32 GetCallstackForCurrentThread( TCallStack** outStack );
        Int32 GetCallstackCopyForCurrentThread( TCallStack** outStack, bool alsoCopyStatics );

        StackTraceInfo( void );
        ~StackTraceInfo();
    };
    
    typedef std::map< UInt32, StackTraceInfo >    TThreadIdToCallStackMap;

    StackTraceInfo* GetStackTraceInfoForCallingThread( MT::CScopeReaderLock& readerLock );
    
    TThreadIdToCallStackMap m_inventory;
    TStackPushCallback m_pushCallback;
    TStackPopCallback m_popCallback;
    char* m_logFilename;
    FILE* m_logFile;
    bool m_logInCvsFormat;
    bool m_logStack;

    MT::CReadWriteLock m_datalock;

    static MT::CMutex g_instanceLock;
    static StackInventory* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex StackInventory::g_instanceLock;
StackInventory* StackInventory::g_instance = GUCEF_NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

StackInventory*
StackInventory::Instance( void )
{
    if ( GUCEF_NULL == g_instance )
    {
        MT::CScopeMutex lock( g_instanceLock );
        if ( GUCEF_NULL == g_instance )
        {
            g_instance = new StackInventory();
        }
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
StackInventory::Deinstance( void )
{
    if ( GUCEF_NULL != g_instance )
    {
        MT::CScopeMutex lock( g_instanceLock );
        if ( GUCEF_NULL != g_instance )
        {
            delete g_instance;
            g_instance = GUCEF_NULL;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
StackInventory::IsInitialized( void )
{
    if ( GUCEF_NULL == g_instance )
    {
        MT::CScopeMutex lock( g_instanceLock );
        if ( GUCEF_NULL == g_instance )
        {
            return false;
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

StackInventory::StackInventory( void )
    : m_inventory()
    , m_pushCallback( GUCEF_NULL )
    , m_popCallback( GUCEF_NULL )
    , m_logFilename( GUCEF_NULL )
    , m_logFile( GUCEF_NULL )
    , m_logInCvsFormat( false )
    , m_logStack( false )
    , m_datalock( true )
{
}

/*-------------------------------------------------------------------------*/

StackInventory::~StackInventory()
{
    MT::CScopeWriterLock writeLock( m_datalock );
    CloseLogFile();
    
    free( m_logFilename );
    m_logFilename = NULL;

    TThreadIdToCallStackMap::iterator i = m_inventory.begin();
    while ( i != m_inventory.end() )
    {
       
        ++i;
    }
    m_inventory.clear();
}

/*-------------------------------------------------------------------------*/

void
StackInventory::CloseLogFile( void )
{
    MT::CScopeWriterLock writeLock( m_datalock );

    if ( ( m_logFile != stdout ) && ( m_logFile != NULL ) )
    {
        fclose( m_logFile );
        m_logFile = NULL;
    }
}

/*-------------------------------------------------------------------------*/

void
StackInventory::FreeCallstackCopy( TCallStack* stackCopy )
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

void 
StackInventory::SetIsStackLoggingEnabled( bool isEnabled )
{
    MT::CScopeWriterLock writeLock( m_datalock );
    m_logStack = isEnabled;
}

/*-------------------------------------------------------------------------*/

void
StackInventory::SetIsStackLoggingInCsvFormat( bool isInCsv )
{
    MT::CScopeWriterLock writeLock( m_datalock );
    m_logInCvsFormat = isInCsv;
}

/*-------------------------------------------------------------------------*/

void
StackInventory::SetLogFilename( const char* filename )
{
    MT::CScopeWriterLock writeLock( m_datalock );

    bool wasOpen = NULL != m_logFile;
    CloseLogFile();

    free( m_logFilename );
    m_logFilename = NULL;

    UInt32 strLen = (UInt32) strlen( filename )+1;
    m_logFilename = (char*) malloc( strLen );
    memcpy( m_logFilename, filename, strLen );

    if ( wasOpen )
    {
        m_logFile = fopen( m_logFilename, "ab" );
    }
}

/*-------------------------------------------------------------------------*/

void
StackInventory::SetStackLogOutputToStdOut( void )
{
    MT::CScopeWriterLock writeLock( m_datalock );

    CloseLogFile();

    free( m_logFilename );
    m_logFilename = NULL;

    m_logFile = stdout;
}

/*-------------------------------------------------------------------------*/

void
StackInventory::Log( const char* logtype ,
                     UInt32 threadID     ,
                     Int32 stackheight   ,
                     const char* file    ,
                     Int32 line          ,
                     UInt32 ticksSpent   )
{
    MT::CScopeReaderLock readLock( m_datalock );

    if ( m_logStack )
    {
        if ( NULL == m_logFile )
        {
            MT::CScopeWriterLock writeLock( readLock );

            /* lazy initialization of the default output file */
            if ( NULL == m_logFilename )
            {
                m_logFilename = (char*) malloc( 14 );
                memcpy( m_logFilename, "Callstack.txt", 14 );
                m_logFile = fopen( m_logFilename, "ab" );
            }
            else
            {
                m_logFile = fopen( m_logFilename, "ab" );
            }

            // Now degrade back to a read lock
            writeLock.TransitionToReader( readLock );
        }

        if ( !m_logInCvsFormat )
        {
            if ( ticksSpent > 0 )
            {
                fprintf( m_logFile, "Thread %d: %s: %d: %s(%d) (%d ms)%s", threadID, logtype, stackheight, file, line, ticksSpent, EOL );
            }
            else
            {
                fprintf( m_logFile, "Thread %d: %s: %d: %s(%d)%s", threadID, logtype, stackheight, file, line, EOL );
            }
        }
        else
        {
            if ( ticksSpent > 0 )
            {
                fprintf( m_logFile, "%d,%s,%d,%s,%d,%d%s", threadID, logtype, stackheight, file, line, ticksSpent, EOL );
            }
            else
            {
                fprintf( m_logFile, "%d,%s,%d,%s,%d%s", threadID, logtype, stackheight, file, line, EOL );
            }
        }
        fflush( m_logFile );
    }
}

/*-------------------------------------------------------------------------*/

StackInventory::StackTraceInfo::StackTraceInfo( void )
    : m_callstack()
{
    memset( &m_callstack, 0, sizeof( m_callstack ) );
}

/*-------------------------------------------------------------------------*/

StackInventory::StackTraceInfo::~StackTraceInfo()
{

}

/*-------------------------------------------------------------------------*/

void
StackInventory::StackTraceInfo::PrintCallstack( FILE* dest )
{
    fprintf( dest, "------------------------------%s", EOL );
    fprintf( dest, "Callstack for thread %d:%s%s", m_callstack.threadid, EOL, EOL );
    fprintf( dest, "stack size = %d%s", m_callstack.items, EOL );
    fprintf( dest, "------------------------------%s%s", EOL, EOL );
    if ( m_callstack.items > 0 )
    {
        for ( UInt32 n=0; n<m_callstack.items; ++n )
        {
            fprintf( dest, "%d: %s(%d)%s", n+1, m_callstack.file[ n ], m_callstack.linenr[ n ], EOL );
        }
        fprintf( dest, "%s%s------------------------------%s%s", EOL, EOL, EOL, EOL );
    }
    else
    {
        fprintf( dest, ">>> no items on stack <<<%s%s", EOL, EOL );
        fprintf( dest, "------------------------------%s%s", EOL, EOL );
    }
}

/*-------------------------------------------------------------------------*/

void
StackInventory::StackTraceInfo::Push( TCallStack* stack ,
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

    //if ( pushCallback != NULL )
    //{
    //    pushCallback( file            ,
    //                  line            ,
    //                  stack->threadid ,
    //                  stack->items    );
    //}
}

/*-------------------------------------------------------------------------*/

void
StackInventory::StackTraceInfo::Pop( TCallStack* stack )
{
    Int32 itemCount;
    if ( stack->items > 0 )
    {
        UInt32 ticksSpent;

        --stack->items;
        itemCount = stack->items;
        ticksSpent = (UInt32) (MT::PrecisionTickCount() - stack->entryTickCount[ itemCount ]);

        //Log( " POP", stack->threadid, itemCount+1, stack->file[ itemCount ], stack->linenr[ itemCount ], ticksSpent );

        //if ( popCallback != NULL )
        //{
        //    popCallback( stack->file[ itemCount ]   ,
        //                 stack->linenr[ itemCount ] ,
        //                 stack->threadid            ,
        //                 stack->items               ,
        //                 ticksSpent                 );
        //}
    }
}

/*-------------------------------------------------------------------------*/

void 
StackInventory::StackTraceInfo::CallstackScopeBegin( const char* file , Int32 line )
{
    Push( &m_callstack, file, line );
}

/*-------------------------------------------------------------------------*/

void 
StackInventory::StackTraceInfo::CallstackScopeEnd( void )
{
    Pop( &m_callstack );
}

/*-------------------------------------------------------------------------*/

Int32
StackInventory::StackTraceInfo::GetCallstackForCurrentThread( TCallStack** outStack )
{
    if ( GUCEF_NULL == outStack )
        return -1;
    *outStack = &m_callstack;
    return 0;
}

/*-------------------------------------------------------------------------*/

Int32
StackInventory::StackTraceInfo::GetCallstackCopyForCurrentThread( TCallStack** outStack, bool alsoCopyStatics )
{
    TCallStack* scopeStack = GUCEF_NULL;
    Int32 result = GetCallstackForCurrentThread( &scopeStack );
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
StackInventory::SetStackPushCallback( TStackPushCallback cBack )
{
    MT::CScopeWriterLock writeLock( m_datalock );
    m_pushCallback = cBack;
}

/*-------------------------------------------------------------------------*/

void
StackInventory::SetStackPopCallback( TStackPopCallback cBack )
{
    MT::CScopeWriterLock writeLock( m_datalock );
    m_popCallback = cBack;
}

/*-------------------------------------------------------------------------*/

StackInventory::StackTraceInfo*
StackInventory::GetStackTraceInfoForCallingThread( MT::CScopeReaderLock& readLock )
{
    UInt32 threadid = MT::GetCurrentTaskID();
    StackTraceInfo* stackTraceInfo = GUCEF_NULL;

    TThreadIdToCallStackMap::iterator i = m_inventory.find( threadid );
    if ( i != m_inventory.end() )
    {
        return &(*i).second;
    }
    else
    {
        // Escalate to a write lock since this is the first time we are seeing this thread
        MT::CScopeWriterLock writeLock( readLock );
        StackTraceInfo& newStack = m_inventory[ threadid ];
        newStack.m_callstack.threadid = threadid;

        assert( 0 == writeLock.GetWriterReentrancyDepth() );

        // Now degrade back to a read lock
        writeLock.TransitionToReader( readLock );
        return &newStack;
    }
}

/*-------------------------------------------------------------------------*/

void 
StackInventory::CallstackScopeBegin( const char* file , Int32 line )
{
    MT::CScopeReaderLock readLock( m_datalock );
    StackTraceInfo* stackTraceInfo = GetStackTraceInfoForCallingThread( readLock );
    stackTraceInfo->CallstackScopeBegin( file, line );
}

/*-------------------------------------------------------------------------*/

void
StackInventory::CallstackScopeEnd( void )
{
    MT::CScopeReaderLock readLock( m_datalock );
    StackTraceInfo* stackTraceInfo = GetStackTraceInfoForCallingThread( readLock );
    stackTraceInfo->CallstackScopeEnd();
}

/*-------------------------------------------------------------------------*/

Int32 
StackInventory::GetCallstackForCurrentThread( TCallStack** outStack )
{
    MT::CScopeReaderLock readLock( m_datalock );
    StackTraceInfo* stackTraceInfo = GetStackTraceInfoForCallingThread( readLock );
    return stackTraceInfo->GetCallstackForCurrentThread( outStack );
}

/*-------------------------------------------------------------------------*/

Int32 
StackInventory::GetCallstackCopyForCurrentThread( TCallStack** outStack, bool alsoCopyStatics )
{
    MT::CScopeReaderLock readLock( m_datalock );
    StackTraceInfo* stackTraceInfo = GetStackTraceInfoForCallingThread( readLock );
    return stackTraceInfo->GetCallstackCopyForCurrentThread( outStack, alsoCopyStatics );
}

/*-------------------------------------------------------------------------*/

void
StackInventory::PrintCallstack( FILE* dest )
{
    // We need to keep everything stable while we print
    // plus the thread that calls print can do so from a thred not associated with the stack
    MT::CScopeWriterLock writeLock( m_datalock );

    TThreadIdToCallStackMap::iterator i = m_inventory.begin();
    while ( i != m_inventory.end() )
    {
        (*i).second.PrintCallstack( dest );
        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
MEMMAN_CallstackScopeBegin( const char* file ,
                            Int32 line       )
{
    StackInventory::Instance()->CallstackScopeBegin( file, line );
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_CallstackScopeEnd( void )
{
    StackInventory::Instance()->CallstackScopeEnd();
}

/*-------------------------------------------------------------------------*/

Int32
MEMMAN_GetCallstackForCurrentThread( TCallStack** outStack )
{
    return StackInventory::Instance()->GetCallstackForCurrentThread( outStack );
}

/*-------------------------------------------------------------------------*/

Int32
MEMMAN_GetCallstackCopyForCurrentThread( TCallStack** outStack  ,
                                         UInt32 alsoCopyStatics )
{
    return StackInventory::Instance()->GetCallstackCopyForCurrentThread( outStack, 0 != alsoCopyStatics );
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_FreeCallstackCopy( TCallStack* stackCopy )
{
    StackInventory::Instance()->FreeCallstackCopy( stackCopy );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_PrintCallstack( void )
{
    StackInventory::Instance()->PrintCallstack( stdout );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_DumpCallstack( const char* filename )
{
    FILE* fptr = NULL;
    fptr = fopen( filename, "wb" );
    if ( NULL != fptr )
    {
        StackInventory::Instance()->PrintCallstack( fptr );
        fclose( fptr );
    }
}

/*-------------------------------------------------------------------------*/

void
GUCEF_ShutdowntCallstackUtility( void )
{
    StackInventory::Deinstance();
}

/*-------------------------------------------------------------------------*/

void
GUCEF_InitCallstackUtility( void )
{
    StackInventory::Instance();
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackLogging( const UInt32 logStackBool )
{
    StackInventory::Instance()->SetIsStackLoggingEnabled( 0 != logStackBool );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_LogStackToStdOut( void )
{
    StackInventory::Instance()->SetStackLogOutputToStdOut();
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackLoggingInCvsFormat( const UInt32 logAsCvsBool )
{
    StackInventory::Instance()->SetIsStackLoggingInCsvFormat( 0 != logAsCvsBool );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackPushCallback( TStackPushCallback cBack )
{
    StackInventory::Instance()->SetStackPushCallback( cBack );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_SetStackPopCallback( TStackPopCallback cBack )
{
    StackInventory::Instance()->SetStackPopCallback( cBack );
}

/*-------------------------------------------------------------------------*/

void
GUCEF_LogStackTo( const char* filename )
{
    StackInventory::Instance()->SetLogFilename( filename );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MLF */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
