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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CSysConsole::CSysConsole( void )
    : CIConfigurable( true )   ,
      CISysConsoleCmdHandler()
{TRACE;

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
{TRACE;

        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CSysConsole::~CSysConsole()
{TRACE;

        DelTree( _root );
}

/*-------------------------------------------------------------------------*/

CSysConsole&
CSysConsole::operator=( const CSysConsole& src )
{TRACE;

        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/

CSysConsole* 
CSysConsole::Instance( void )
{TRACE;

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
{TRACE;
       
        _datalock.Lock();
        delete _instance;
        _instance = NULL;
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CSysConsole::DelTree( TCmdChannel* tree )
{TRACE;

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
{TRACE;

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
{TRACE;

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
{TRACE;

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
{TRACE;

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
{TRACE;

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
{TRACE;
                        
}

/*-------------------------------------------------------------------------*/                            

void
CSysConsole::InitClient( CSysConsoleClient* client )
{TRACE;

        _datalock.Lock();
        client->channel = _root;
        ++_root->views;
        _datalock.Unlock(); 
}

/*-------------------------------------------------------------------------*/

void 
CSysConsole::UnregClient( CSysConsoleClient* client )
{TRACE;

        _datalock.Lock();
        TCmdChannel* channel = static_cast< TCmdChannel* >( client->channel );
        
        //++_root->views;
        
        // makeme
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void 
CSysConsole::LeaveDir( CSysConsoleClient* client )
{TRACE;

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
{TRACE;

        _datalock.Lock();
        TCmdChannel* channel = FindChannel( static_cast< TCmdChannel* >( client->channel ) , 
                                            dirname                                        );
        if ( channel )
        {
                client->channel = channel;
                client->_path += ( '\\' + dirname );
                
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
{TRACE;
    
    // sanity check on the input
    if ( client == NULL || path.Length() == 0 ) return false;
    
    _datalock.Lock();
    
    CString thePath( path );
    struct SCmdChannel* channel = NULL;
    
    // Check if we should jump from the root or from the client's current dir
    if ( path[ 0 ] == '\\' )
    {
        // we start jumping from the root
        channel = _root;

        // Set the client dir to root
        client->channel = channel;
        client->_path = "";        

        if ( path.Length() == 1 )
        {
            client->channel = channel;
            _datalock.Unlock();
            return true;            
        }
        else
        {   
            thePath = thePath.CutChars( 1, true );
        }
    }
    else
    {
        // we start jumping from the client's current dir
        channel = static_cast< TCmdChannel* >( client->channel );
    }
    
    CString dirname = thePath.SubstrToChar( '\\' );
    
    // Loop trough all the directories in the path
    while ( thePath.Length() > 0 )
    {
        channel = FindChannel( channel , 
                               dirname );
                                            
        if ( channel != NULL )
        {
            if ( thePath.Length() > dirname.Length() )
            {
                // there are more sub dirs, prepare for another loop
                thePath = thePath.CutChars( dirname.Length()+1, true );
                dirname = thePath.SubstrToChar( '\\' );
            }
            else
            {
                // have have processed the last sub dir
                client->channel = channel;
                _datalock.Unlock();
                return true;
            }
        }
        else
        {
            // the path is invalid
            _datalock.Unlock();
            return false;
        }
    }

    // If we get here then the input was most likely invalid
    _datalock.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

CSysConsole::TAliasData* 
CSysConsole::FindAliasFunction( const CString& aliasname )
{TRACE;

    TAliasList::iterator i = _aliases.find( aliasname );
    if ( i != _aliases.end() )
    {
        return &(*i).second;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::Execute( CSysConsoleClient* client  ,
                      const CString& funcname    ,
                      const CStringList& arglist ,
                      CStringList& resultdata    )
{TRACE;

        _datalock.Lock();
        
        /*
         *      First check the alias list
         */
        TAliasData* ad = FindAliasFunction( funcname );
        if ( ad != NULL )
        {
            // Jump to the path represented by the alias
            if ( JumpTo( client, ad->path ) )
            {
                // Execute the actual function based on the alias
                bool success = Execute( client       ,
                                        ad->function ,
                                        arglist      ,
                                        resultdata   );
                _datalock.Lock();
                return success;
            }
            
            _datalock.Lock();
            return false;
        }
        
        /*
         *      Now check the functions in the client's current namespace dir
         */
        TFunctionHook* func = FindFunction( static_cast< TCmdChannel* >( client->channel ) ,
                                            funcname                                       ); 
                                             
        if ( func )
        {                                            
            if ( arglist.GetCount() == func->argdefs.GetCount() )
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
            else
            {
                resultdata.Append( "ERROR> Expected " + Int32ToString( func->argdefs.GetCount() ) + " values" );
                resultdata.Append( Int32ToString( arglist.GetCount() ) + " values where entered" );
                
                if ( func->argdefs.GetCount() > 0 )
                {
                    resultdata.Append( "Expected values are:" );
                    for ( UInt32 i=0; i<func->argdefs.GetCount(); ++i )
                    {
                        resultdata.Append( "   " + func->argdefs[ i ] );
                    }
                }
            }
        }                                                                 
        _datalock.Unlock();        
        return false;      
}

/*-------------------------------------------------------------------------*/                      

CStringList 
CSysConsole::GetDirList( const CSysConsoleClient* client ) const
{TRACE;

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
{TRACE;

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
{TRACE;       
        
    _datalock.Lock(); 
                
    // First make sure the alias is unique and not already registered
    TAliasList::iterator i = _aliases.find( aliasname );
    if ( i != _aliases.end() )
    {
        _datalock.Unlock();
        return false;        
    }
    
    // if we get here then the alias does not exist yet and can be registered        
    TAliasData ad;
    ad.function = function;
    ad.path = path;    

    _aliases.insert( std::pair< CString, TAliasData >( aliasname, ad ) );
    
    _datalock.Unlock();
    return true;
}                            

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::UnregisterAlias( const CString& aliasname ,
                              const CString& path      ,
                              const CString& function  )
{TRACE;

    _datalock.Lock(); 
                
    // First make sure the alias is actually registered
    TAliasList::iterator i = _aliases.find( aliasname );
    if ( i != _aliases.end() )
    {
        // delete the alias entry
        _aliases.erase( i );
        _datalock.Unlock();
        return true;        
    }
    
    // Unable to find the alias
    _datalock.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsole::OnSysConsoleCommand( const CString& path     ,
                                  const CString& command  ,
                                  const CStringList& args ,
                                  CStringList& resultdata )
{TRACE;

    if ( command == "RegisterAlias" )
    {
        if ( args.GetCount() >= 3 )
        { 
            return RegisterAlias( args[ 0 ] ,
                                  args[ 1 ] ,
                                  args[ 2 ] );
        }
        return false;
    }        
    if ( command == "UnregisterAlias" )
    {               
        if ( args.GetCount() >= 3 )
        { 
            return UnregisterAlias( args[ 0 ] ,
                                    args[ 1 ] ,
                                    args[ 2 ] );
        }
        return false;
    }
    return false;        
}                                  

/*-------------------------------------------------------------------------*/

bool
CSysConsole::SaveConfig( CDataNode& tree )
{TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CSysConsole::LoadConfig( const CDataNode& treeroot )
{TRACE;

    const GUCEF::CORE::CDataNode* n = treeroot.Find( "ConsoleAliasList" );
    if ( n != NULL )
    {
        const GUCEF::CORE::CDataNode* m = NULL;
        GUCEF::CORE::CDataNode::const_iterator i = n->ConstBegin();
        while ( i != n->ConstEnd() )
        {
            m = (*i);
            if ( m->GetName() == "Alias" )
            {
                // Found an alias entry, parse values and add the alias
                const GUCEF::CORE::CDataNode::TNodeAtt* att = m->GetAttribute( "name" );
                if ( att != NULL )
                {
                    CString aliasName = att->value;
                    att = m->GetAttribute( "path" );
                    if ( att != NULL )
                    {
                        CString path = att->value;
                        att = m->GetAttribute( "function" );
                        if ( att != NULL )
                        {
                            RegisterAlias( aliasName  ,
                                           path       ,
                                           att->value );
                        }
                    }
                }
            }
            ++i;
        }
    }
    
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
