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

#include "CEventPump.h"
#include "CEvent.h"
#include "CEventTypeRegistry.h"
#include "CDataNode.h"
#include "dvoswrap.h"

#ifndef DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */ 
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"     /* gucefVFS Library macros */
#define GUCEFVFS_MACROS_H
#endif /* GUCEFVFS_MACROS_H ? */

#include "CFPCodecPluginManager.h"

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CFPCodecPluginManager* CFPCodecPluginManager::_instance = NULL;
MT::CMutex CFPCodecPluginManager::_mutex; 

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CFPCodecPluginManager::CFPCodecPluginManager( void )
        : CIConfigurable( true )
{TRACE;
        DEBUGOUTPUT( "CFPCodecPluginManager::CFPCodecPluginManager( void )" );
        
        /*
         *      Register event types
         */
        CORE::CEventTypeRegistry* etr = CORE::CEventTypeRegistry::Instance();
        _codecaddevent = etr->RegisterType( "GUCEF::VFS::CFPCODECMANAGER::FPCODECADD" ,
                                            sizeof(TFPCodedAdded)                     );
        _codecdelevent = etr->RegisterType( "GUCEF::VFS::CFPCODECMANAGER::FPCODECDEL" ,
                                            sizeof(TFPCodedDeleted)                   ); 
}

/*-------------------------------------------------------------------------*/

CFPCodecPluginManager::CFPCodecPluginManager( const CFPCodecPluginManager& src )
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::CFPCodecManager( const CFPCodecManager& src )" );
}

/*-------------------------------------------------------------------------*/
                
CFPCodecPluginManager::~CFPCodecPluginManager()
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::~CFPCodecManager()" );
}

/*-------------------------------------------------------------------------*/
        
CFPCodecPluginManager& 
CFPCodecPluginManager::operator=( const CFPCodecPluginManager& src )
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::operator=( const CFPCodecManager& src )" );
        
        return *_instance;
}

/*-------------------------------------------------------------------------*/

CFPCodecPluginManager*
CFPCodecPluginManager::Instance( void )
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::Instance( void )" );
        
        _mutex.Lock();
        if ( !_instance )
        {
                _instance = new CFPCodecPluginManager();        
        }
        _mutex.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void
CFPCodecPluginManager::Deinstance( void )
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::Deinstance( void )" );
        
        _mutex.Lock();
        delete _instance;
        _instance = NULL;
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool 
CFPCodecPluginManager::IsPluginLoaded( const CORE::CString& path )
{TRACE;
        UInt32 count = _codecs.GetCount();
        CFPCodecPlugin* fpcp;
        for ( UInt32 i=0; i<count; ++i )
        {
                fpcp = static_cast<CFPCodecPlugin*>(_codecs[ i ]);
                if ( fpcp->GetLocation() == path )
                {
                        return true;
                }
        }
        return false; 
}

/*-------------------------------------------------------------------------*/

/**
 *      Attempts to load a codec file which is a dynamicly linked
 *      library.
 *
 *      Dispatches the following events:
 *              - "GUCEF::VFS::CFPCODECMANAGER::FPCODECADD"
 *                      This event has the following data:
 *                              - TFPCodedAdded        
 */
CFPCodecPlugin* 
CFPCodecPluginManager::AddCodec( const CORE::CString& filename )
{TRACE;
        _mutex.Lock();
        
        CORE::CString path( RelativePath( filename ) );
        if ( !IsPluginLoaded( path ) )
        {        
                CFPCodecPlugin* codec = new CFPCodecPlugin(  );
                if ( codec->IsValid() )
                {
                        Int32 idx = _codecs.AddEntry( codec );
                        
                        /*
                        *      Send an event notifying the rest of the system 
                        *      that a new pack codec is available
                        */
                        CORE::CEvent codecaddevent( _codecaddevent );                
                        TFPCodedAdded data;
                        data.filenamestr = filename.Cache();
                        data.index = idx;
                        data.namestr = codec->GetName().Cache();
                        codecaddevent.SetData( &data                  ,
                                        sizeof(TFPCodedAdded) ); 
                        CORE::CEventPump::Instance()->SendEvent( codecaddevent );
                        
                        _mutex.Unlock();
                        return codec;
                }
                else
                {
                        delete codec;                        
                }
        }
        _mutex.Unlock();
        return NULL;                
}

/*-------------------------------------------------------------------------*/

/**
 *      Attempts to delete the given codec.
 *      Note that deleting a codec will cause the dynamicly linked
 *      codec module to be unloaded. A codec module cannot be unloaded
 *      unless it's loadcount is zero. So be sure to use
 *      UnloadImageData() as often as LoadImageData().
 *
 *      Dispatches the following events:
 *              - "GUCEF::IMAGE::IMGCODECDEL"   
 *                      This event has the following data:
 *                              - TFPCodedDeleted     
 */
bool 
CFPCodecPluginManager::DeleteCodec( CFPCodecPlugin* codec )
{TRACE;
        return false;
}

/*-------------------------------------------------------------------------*/
        
CORE::CStringList 
CFPCodecPluginManager::GetCodecExtList( void ) const
{TRACE;
        CORE::CStringList list;
        return list;
}

/*-------------------------------------------------------------------------*/
        
CFPCodecPlugin* 
CFPCodecPluginManager::GetCodec( UInt32 codecidx ) const
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::GetCodec( UInt32 codecidx ) const" );
        
        _mutex.Lock();
        CFPCodecPlugin* cp = static_cast<CFPCodecPlugin*>( _codecs[ codecidx ] );
        _mutex.Unlock();
        return cp;
}

/*-------------------------------------------------------------------------*/
        
CFPCodecPlugin* 
CFPCodecPluginManager::GetCodec( const CORE::CString fileext ) const
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::GetCodec( const CORE::CString fileext ) const" );
        
        return NULL;
}
        
/*-------------------------------------------------------------------------*/        
        
void 
CFPCodecPluginManager::LoadAll( void )
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::LoadAll( void )" );
        
        _mutex.Lock();        
        
        CORE::CString file;
        CORE::CString filepath; 
        CORE::CString path = GetPluginDir();
        
        struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( path.C_String() );
        if ( did )
        {
                do {                
                        if ( CORE::DI_Is_It_A_File( did ) )
                        {
                                DEBUGOUTPUTsss( "Attempt to load ", DI_Name( did ), " as an VFSCodec plugin" );
                                
                                filepath = path; 
                                file = DI_Name( did );
                                CORE::AppendToPath( filepath, file );
                                AddCodec( filepath );
                        }
                }
                while ( CORE::DI_Next_Dir_Entry( did ) != 0 );
                
                CORE::DI_Cleanup( did );
        }
        
        _mutex.Unlock();        
}
        
/*-------------------------------------------------------------------------*/        
        
void 
CFPCodecPluginManager::UnloadAll( void )
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::UnloadAll( void )" );
}

/*-------------------------------------------------------------------------*/

void 
CFPCodecPluginManager::OnSetPluginDir( const CORE::CString& path )
{TRACE;
        DEBUGOUTPUT( "CFPCodecManager::OnSetPluginDir( const CORE::CString& path  )" );       
}

/*-------------------------------------------------------------------------*/

bool 
CFPCodecPluginManager::SaveConfig( CORE::CDataNode& tree )
{TRACE;
        _mutex.Lock();
        CORE::CDataNode* n = tree.Structure( "GUCEF%VFS%CFPCodecPluginManager" ,
                                             '%'                               );
        UInt32 count = _codecs.GetCount();
        CORE::CDataNode::TNodeAtt att;
                                                
        CORE::CString tmp;
        tmp.SetInt( count );                                        
        n->SetAttribute( "plugincount", tmp );                                        
                                                             
        CORE::CDataNode plugin( "CFPCodecPlugin" );
        CFPCodecPlugin* cp;        
        for ( UInt32 i=0; i<count; ++i )
        {
                cp = static_cast<CFPCodecPlugin*>(_codecs[ i ]);
                plugin.SetAttribute( "name", cp->GetName() );
                //plugin.SetAttribute( "type", cp->GetTypeName() );
                plugin.SetAttribute( "path", cp->GetLocation() );
                plugin.SetAttribute( "version", CORE::VersionToString( cp->GetVersion() ) );
                plugin.SetAttribute( "copyright", cp->GetCopyrightEULA() );                
                n->AddChild( plugin );
        }
        _mutex.Unlock();                        
        return true;
}

/*-------------------------------------------------------------------------*/

bool 
CFPCodecPluginManager::LoadConfig( const CORE::CDataNode& tree )
{TRACE;
        _mutex.Lock();
        CORE::CDataNode* n = tree.Search( "GUCEF%VFS%CFPCodecPluginManager" , 
                                          '%'                               ,
                                          false                             );
        if ( n )
        {
                CORE::CString cmpstr( "CFPCodecPlugin" );
                CFPCodecPlugin* fpp;
                CORE::CString path( "path" );
                const CORE::CDataNode* c;
                const CORE::CDataNode::TNodeAtt* att; 
                CORE::CDataNode::const_iterator i = n->ConstBegin();
                while ( i != n->ConstEnd() )
                {
                        c = (*i);
                        if ( c->GetName() == cmpstr )
                        {
                                att = c->GetAttribute( path );
                                if ( att )
                                {
                                        /*
                                         *      Test if this module is already loaded
                                         */
                                        bool found = false;
                                        UInt32 count = _codecs.GetCount();
                                        for ( UInt32 n=0; n<count; ++n )
                                        {
                                                fpp = static_cast<CFPCodecPlugin*>(_codecs[ n ]);
                                                if ( att->value == fpp->GetLocation() )
                                                {       
                                                        found = true;                                                 
                                                        break;        
                                                }                                                                                                
                                        }
                                        if ( !found )
                                        {
                                                /*
                                                 *      This module is not loaded yet.
                                                 *      We will do so now.
                                                 */
                                                AddCodec( att->value ); 
                                        }                                                
                                }       
                        }
                        ++i;
                }
        }                                     
        _mutex.Unlock();                                     
        return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
