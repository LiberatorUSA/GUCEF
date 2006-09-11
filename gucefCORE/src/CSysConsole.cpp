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

#ifndef GUCEF_CORE_CISYSCONSOLECMDHANDLER_H
#include "CISysConsoleCmdHandler.h"
#define GUCEF_CORE_CISYSCONSOLECMDHANDLER_H
#endif /* GUCEF_CORE_CISYSCONSOLECMDHANDLER_H ? */

#ifndef GUCEF_CORE_CSYSCONSOLECLIENT_H
#include "CSysConsoleClient.h"
#define GUCEF_CORE_CSYSCONSOLECLIENT_H 
#endif /* GUCEF_CORE_CSYSCONSOLECLIENT_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#include "CSysConsole.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CSysConsole* CSysConsole::_instance = NULL;
MT::CMutex CSysConsole::_datalock;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct CSysConsole::SCmdChannel
{
        CString name;
        CDynamicArray channels;
        CDynamicArray functions;
        struct SCmdChannel* parent;
        UInt32 views;
};
typedef struct CSysConsole::SCmdChannel TCmdChannel;

/*-------------------------------------------------------------------------*/

struct CSysConsole::SFunctionHook
{
        CString name;
        CStringList argdefs;
        CISysConsoleCmdHandler* handler;                
};
typedef struct CSysConsole::SFunctionHook TFunctionHook;

/*-------------------------------------------------------------------------*/

struct SAliasData
{
        CString cmd;
        CString path;
        CString function;
};
typedef struct SAliasData TAliasData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CSysConsole::CSysConsole( void )
{
        _root = new TCmdChannel;
        _root->name = "root";
        _root->parent = NULL;
        
        /*
         *      The first functions are registered by the console
         *      itself. The functions allow you to register aliases
         *      using the console.
         */
        CStringList args;
        args.Append( "aliasname" );
        args.Append( "path" );
        args.Append( "function" );
        RegisterCmd( "GUCEF\\CORE\\CSysConsole" ,
                     "RegisterAlias"            ,
                     args                       ,
                     this                       );
        args.Clear();
        args.Append( "aliasname" );             
        RegisterCmd( "GUCEF\\CORE\\CSysConsole" ,
                     "UnregisterAlias"          ,
                     args                       ,
                     this                       );                     
}

/*-------------------------------------------------------------------------*/

CSysConsole::CSysConsole( const CSysConsole& src )
{
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CSysConsole::~CSysConsole()
{
        DelTree( _root );
}

/*-------------------------------------------------------------------------*/

CSysConsole&
CSysConsole::operator=( const CSysConsole& src )
{
        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/

CSysConsole* 
CSysConsole::Instance( void )
{
        _datalock.Lock();
        if ( !_instance )
        {
                _instance = new CSysConsole();
        }                
        _datalock.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CSysConsole::Deinstance( void )
{       
        _datalock.Lock();
        delete _instance;
        _instance = NULL;
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CSysConsole::DelTree( TCmdChannel* tree )
{
        CDynamicArray* array = &tree->channels;
        TCmdChannel* chentry;
        for ( UInt32 i=0; i<array->GetCount(); ++i )
        {
                chentry = static_cast<TCmdChannel*>( (*array)[ i ] );
                DelTree( chentry );
                delete chentry;
        }
        array->Clear();
        array = &tree->functions;
        TFunctionHook* fhook; 
        for ( UInt32 i=0; i<array->GetCount(); ++i )
        {
                fhook = static_cast<TFunctionHook*>( (*array)[ i ] );
                delete fhook;
        }
        array->Clear();
        
        delete tree; // is this safe ?                                
}

/*-------------------------------------------------------------------------*/

struct CSysConsole::SCmdChannel* 
CSysConsole::FindChannel( struct SCmdChannel* curchannel ,
                          const CString& name            ) 
{
        const CDynamicArray* channels = &curchannel->channels;
        TCmdChannel* chentry;
        for ( UInt32 i=0; i<channels->GetCount(); ++i )
        {
                chentry = static_cast<TCmdChannel*>( (*channels)[ i ] );
                if ( chentry->name == name )
                {
                        return chentry;
                }
        }
        return NULL;        
}

/*-------------------------------------------------------------------------*/

struct CSysConsole::SCmdChannel* 
CSysConsole::WalkTree( struct SCmdChannel* curchannel ,
                       const CString& path            ,
                       CString& leftover              ) 
{
        CString chname( path.SubstrToChar( '\\', true ) );        
        TCmdChannel* channel = FindChannel( curchannel, chname );
        if ( channel )
        {
                TCmdChannel* subch;
                leftover = path.CutChars( chname.Length()+1, true );
                CString orgleftover( leftover );
                subch = WalkTree( channel     ,
                                  orgleftover ,
                                  leftover    );
                if ( subch )
                {
                        return subch;
                }
                leftover = orgleftover;
                return channel;                                          
        }
        leftover = path;
        return curchannel;       
}

/*-------------------------------------------------------------------------*/

struct CSysConsole::SCmdChannel* 
CSysConsole::BuildTree( struct SCmdChannel* curchannel ,
                        const CString& path            )
{
        CString leftover;
        TCmdChannel* channel = WalkTree( curchannel ,
                                         path       , 
                                         leftover   );
        if ( leftover.Length() )
        {
                TCmdChannel* parent = channel;
                TCmdChannel* newch;
                do
                { 
                        newch = new TCmdChannel;
                        newch->name = leftover.SubstrToChar( '\\', true );
                        newch->parent = parent;
                        newch->views = 0;
                        parent->channels.AppendEntry( newch );
                        parent = newch;
                        leftover = leftover.CutChars( newch->name.Length()+1, true );
                }
                while ( leftover.Length() );
                return newch;
        }
        return channel;
                                                        
}                        

/*-------------------------------------------------------------------------*/

struct CSysConsole::SFunctionHook*
CSysConsole::FindFunction( const struct SCmdChannel* curchannel ,
                           const CString& funcname              ) 
{
        const CDynamicArray* commands = &curchannel->functions;
        TFunctionHook* cmdentry;        
        for ( UInt32 i=0; i<commands->GetCount(); ++i )
        {
                cmdentry = static_cast<TFunctionHook*>( (*commands)[ i ] );
                if ( cmdentry->name == funcname )
                {
                        return cmdentry;
                }
        }
        return NULL;        
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::RegisterCmd( const CString& path                ,
                          const CString& functionname        ,
                          const CStringList& args            ,
                          CISysConsoleCmdHandler* cmdhandler )
{
        if ( functionname.HasChar( ' ', true ) < 0 )
        {
                CString thepath( path.ReplaceChar( '/', '\\' ) );

                _datalock.Lock();
                TCmdChannel* channel = BuildTree( _root   ,
                                                  thepath );
                TFunctionHook* func = FindFunction( channel      , 
                                                    functionname );
                if ( !func )
                {
                        func = new TFunctionHook;
                        func->name = functionname;
                        func->argdefs = args;
                        func->handler = cmdhandler;
                        
                        channel->functions.AppendEntry( func );
                        
                        _datalock.Unlock();
                        return true;
                }                                                                           
                _datalock.Unlock();
                return false;
        }
        return false;                
}

/*-------------------------------------------------------------------------*/                          

void 
CSysConsole::UnregisterCmd( const CString& path         ,
                            const CString& functionname )
{
                        
}

/*-------------------------------------------------------------------------*/                            

void
CSysConsole::InitClient( CSysConsoleClient* client )
{
        _datalock.Lock();
        client->channel = _root;
        ++_root->views;
        _datalock.Unlock(); 
}

/*-------------------------------------------------------------------------*/

void 
CSysConsole::UnregClient( CSysConsoleClient* client )
{
        _datalock.Lock();
        TCmdChannel* channel = static_cast< TCmdChannel* >( client->channel );
        
        //++_root->views;
        
        // makeme
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void 
CSysConsole::LeaveDir( CSysConsoleClient* client )
{
        _datalock.Lock();
        TCmdChannel* curchannel = static_cast< TCmdChannel* >( client->channel );
        if ( curchannel->parent )
        {
                client->channel = curchannel->parent;                     
                --static_cast< TCmdChannel* >( client->channel )->views;
        }
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::EnterDir( CSysConsoleClient* client ,
                       const CString& dirname    )
{
        _datalock.Lock();
        TCmdChannel* channel = FindChannel( static_cast< TCmdChannel* >( client->channel ) , 
                                            dirname                                        );
        if ( channel )
        {
                client->channel = channel;
                client->_path += "\\";
                client->_path += dirname;
                
                ++channel->views;
                                    
                _datalock.Unlock();
                return true;                
        }
        _datalock.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/                       

bool 
CSysConsole::JumpTo( CSysConsoleClient* client ,
                     const CString& path       )
{
        return false;
}

/*-------------------------------------------------------------------------*/

struct CSysConsole::SFunctionHook* 
CSysConsole::FindAliasFunction( const CString& aliasname )
{
        TAliasData* ad;
        for ( UInt32 i=0; i<_aliases.GetCount(); ++i )
        {
                ad = static_cast< TAliasData* >( _aliases[ i ] );                
                if ( ad->cmd == aliasname )
                {
                        CString leftover;
                        TCmdChannel* channel = WalkTree( _root    ,
                                                         ad->path ,
                                                         leftover );
                        if ( channel && !leftover.Length() )
                        {
                                return FindFunction( channel      , 
                                                     ad->function );
                        }
                        return NULL;                                                
                }
        }
        return NULL;
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::Execute( CSysConsoleClient* client  ,
                      const CString& funcname    ,
                      const CStringList& arglist ,
                      CStringList& resultdata    )
{
        _datalock.Lock();
        
        /*
         *      First check the alias list
         */
        TFunctionHook* func = FindAliasFunction( funcname );
        if ( !func )
        {
                /*
                 *      Now check the functions in the client's current namespace dir
                 */
                func = FindFunction( static_cast< TCmdChannel* >( client->channel ) ,
                                     funcname                                       );                
        }
        
        if ( func )
        {                                            
                if ( func->handler->OnSysConsoleCommand( client->_path ,
                                                         funcname      ,
                                                         arglist       ,
                                                         resultdata    ) )
                {
                        _datalock.Unlock(); 
                        return true;        
                }
        }                                                                 
        _datalock.Unlock();        
        return false;      
}

/*-------------------------------------------------------------------------*/                      

CStringList 
CSysConsole::GetDirList( const CSysConsoleClient* client ) const
{
        _datalock.Lock();
        CDynamicArray* channels = &(static_cast< TCmdChannel* >( client->channel )->channels);
        CStringList list;
        for ( UInt32 i=0; i<channels->GetCount(); ++i )
        {
                list.Append( static_cast<TCmdChannel*>( (*channels)[ i ] )->name );                
        }
        _datalock.Unlock();
        return list;        
}

/*-------------------------------------------------------------------------*/

CStringList 
CSysConsole::GetCmdList( const CSysConsoleClient* client ) const
{
        _datalock.Lock();
        CDynamicArray* functions = &(static_cast< TCmdChannel* >( client->channel )->functions);
        CStringList list;
        for ( UInt32 i=0; i<functions->GetCount(); ++i )
        {
                list.Append( static_cast<TFunctionHook*>( (*functions)[ i ] )->name );                
        }
        _datalock.Unlock();
        return list; 
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::RegisterAlias( const CString& aliasname ,
                            const CString& path      ,
                            const CString& function  )
{
        TAliasData* ad = new TAliasData;        
        ad->cmd = aliasname;
        ad->function = function;
        ad->path = path;
        
        _datalock.Lock(); 
        
        for ( UInt32 i=0; i<_aliases.GetCount(); ++i )
        {
                ad = static_cast< TAliasData* >( _aliases[ i ] );                
                if ( ad->cmd == aliasname )
                {
                        delete ad;
                        _datalock.Unlock();
                        return false;                                                
                }
        }        
               
        _aliases.AppendEntry( ad );
        _datalock.Unlock();
        return true;
}                            

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::UnregisterAlias( const CString& aliasname ,
                              const CString& path      ,
                              const CString& function  )
{
        TAliasData* ad = new TAliasData;        
        ad->cmd = aliasname;
        ad->function = function;
        ad->path = path;

        _datalock.Lock(); 

        for ( UInt32 i=0; i<_aliases.GetCount(); ++i )
        {
                ad = static_cast< TAliasData* >( _aliases[ i ] );                
                if ( ad->cmd == aliasname )
                {
                        _aliases.SetEntry( i, NULL );
                        delete ad;
                        _aliases.FillNULLGaps();
                        _datalock.Unlock();
                        return true;                                                
                }
        }        
        _datalock.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::OnSysConsoleCommand( const CString& path     ,
                                  const CString& command  ,
                                  const CStringList& args ,
                                  CStringList& resultdata )
{
        if ( command == "RegisterAlias" )
        {
                return true;
        }        
        if ( command == "UnregisterAlias" )
        {               
                return true;
        }
        return false;        
}                                  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
