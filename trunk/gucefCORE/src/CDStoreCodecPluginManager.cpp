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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */ 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"                /* file handling utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"       /* registry for dstore codecs */
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"          /* node for data storage */
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#include "CDStoreCodecPluginManager.h"  /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

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

MT::CMutex CDStoreCodecPluginManager::_datalock;
CDStoreCodecPluginManager* CDStoreCodecPluginManager::_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDStoreCodecPluginManager::CDStoreCodecPluginManager( void )
        : CIConfigurable( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDStoreCodecPluginManager::~CDStoreCodecPluginManager()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

bool 
CDStoreCodecPluginManager::IsPluginLoaded( const CString& path )
{GUCEF_TRACE;
        UInt32 count = _codecs.GetCount();
        CDStoreCodecPlugin* cp;
        for ( UInt32 i=0; i<count; ++i )
        {
                cp = static_cast<CDStoreCodecPlugin*>(_codecs[ i ]);
                if ( cp->GetModulePath() == path )
                {
                        return true;
                }
        }
        return false; 
}

/*-------------------------------------------------------------------------*/

void 
CDStoreCodecPluginManager::LoadAll( void )
{
    LoadAll( GetPluginDir() );
}

/*-------------------------------------------------------------------------*/

void
CDStoreCodecPluginManager::LoadAll( const CString& pluginDir )
{GUCEF_TRACE;

    _datalock.Lock();        
    
    CORE::CString file;
    CORE::CString filepath;
    
    struct SDI_Data* did = DI_First_Dir_Entry( pluginDir.C_String() );
    if ( 0 != did )
    {
        do 
        {                
            if ( DI_Is_It_A_File( did ) )
            {       
                file = DI_Name( did );
                if ( 0 == file.HasSubstr( "dstoreplugin" ) && ExtractFileExtention( file ).Equals( "dll", false ) )
                {                                
                    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Attempt to load " + file + " as an DSTORE Codec plugin" );
                    
                    filepath = pluginDir;                                     
                    AppendToPath( filepath, file );
                    LoadCodecPlugin( filepath );
                }
            }
        }
        while ( DI_Next_Dir_Entry( did ) != 0 );
        
        DI_Cleanup( did );
    }
    
    _datalock.Unlock();       
}
        
/*-------------------------------------------------------------------------*/        
        
void 
CDStoreCodecPluginManager::UnloadAll( void )
{GUCEF_TRACE;

}
        
/*-------------------------------------------------------------------------*/
        
void 
CDStoreCodecPluginManager::OnSetPluginDir( const CString& path )
{GUCEF_TRACE;
      
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPluginManager*
CDStoreCodecPluginManager::Instance( void )
{GUCEF_TRACE;
        _datalock.Lock();
        if ( !_instance )
        {
                _instance = new CDStoreCodecPluginManager();
                CHECKMEM( _instance, sizeof(CDStoreCodecPluginManager) );
                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CDStoreCodecPluginManager Singleton created" );
        }
        _datalock.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CDStoreCodecPluginManager::Deinstance( void )
{GUCEF_TRACE;
        _datalock.Lock();
        CHECKMEM( _instance, sizeof(CDStoreCodecPluginManager) );
        delete _instance;
        _instance = NULL;
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CDStoreCodecPluginManager Singleton destroyed" );
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

CPluginManager::TPluginPtr
CDStoreCodecPluginManager::LoadPlugin( const CString& pluginPath )
{GUCEF_TRACE;

    return TPluginPtr( LoadCodecPlugin( pluginPath ) );
}

/*-------------------------------------------------------------------------*/

CDStoreCodecPlugin* 
CDStoreCodecPluginManager::LoadCodecPlugin( const CString& filename )
{GUCEF_TRACE;

    CString path = RelativePath( filename );
    if ( !IsPluginLoaded( path ) )
    {
        CDStoreCodecPlugin* cp = new CDStoreCodecPlugin( path );
        CHECKMEM( cp, sizeof(CDStoreCodecPlugin) );
        if ( cp->IsValid() )
        {
            _datalock.Lock();
            cp->SetPluginID( _codecs.AddEntry( cp ) );                
            _datalock.Unlock();
            CDStoreCodecRegistry::Instance()->Register( cp->GetTypeName().C_String() , 
                                                        &cp->_ref                    );
            
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "Loaded Data Storage codec plugin with name: " + cp->GetTypeName() );
            return cp;
        }
        delete cp;
    }                
    return NULL;
}

/*-------------------------------------------------------------------------*/
        
void 
CDStoreCodecPluginManager::UnloadCodecPlugin( CDStoreCodecPlugin* plugin )
{GUCEF_TRACE;
        if ( plugin )
        {
                _datalock.Lock();
                CHECKMEM( plugin, sizeof(CDStoreCodecPlugin) );
                _codecs.SetEntry( plugin->GetPluginID(), NULL );                
                delete plugin;
                _datalock.Unlock();
                CDStoreCodecRegistry::Instance()->Unregister( plugin->GetTypeName() );
        }               
}

/*-------------------------------------------------------------------------*/
        
CDStoreCodecPlugin* 
CDStoreCodecPluginManager::GetCodec( const CString& codectype ) const
{GUCEF_TRACE;
        _datalock.Lock();
        UInt32 count = _codecs.GetCount();
        CDStoreCodecPlugin* cp;
        for ( UInt32 i=0; i<count; ++i )
        {
                cp = static_cast<CDStoreCodecPlugin*>(_codecs[ i ]);
                CHECKMEM( cp, sizeof(CDStoreCodecPlugin) );
                if ( cp->GetTypeName() == codectype )
                {
                        _datalock.Unlock();
                        return cp;
                }
        }        
        _datalock.Unlock();
        return NULL;
}

/*-------------------------------------------------------------------------*/

bool 
CDStoreCodecPluginManager::SaveConfig( CDataNode& tree )
{GUCEF_TRACE;
        _datalock.Lock();
        CDataNode* n = tree.Structure( "GUCEF%CORE%CDStoreCodecPluginManager" ,
                                       '%'                                    );

        UInt32 count = _codecs.GetCount();                                        
        n->SetAttribute( "plugincount", UInt32ToString( count ) );                                        
                               
        n->DelSubTree();
        CDataNode plugin( "CDStoreCodecPlugin" );
        CDStoreCodecPlugin* cp;        
 
        for ( UInt32 i=0; i<count; ++i )
        {
                cp = static_cast<CDStoreCodecPlugin*>(_codecs[ i ]);
                plugin.SetAttribute( "name", cp->GetName() );
                plugin.SetAttribute( "type", cp->GetTypeName() );
                plugin.SetAttribute( "version", VersionToString( cp->GetVersion() ) );
                plugin.SetAttribute( "path", cp->GetModulePath() );
                plugin.SetAttribute( "copyright", cp->GetCopyright() );
                n->AddChild( plugin );
        }
        _datalock.Unlock();                         
        return true;
}

/*-------------------------------------------------------------------------*/
                   
bool 
CDStoreCodecPluginManager::LoadConfig( const CDataNode& tree )
{GUCEF_TRACE;
        
        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CDStoreCodecPluginManager: Loading config" );
        
        _datalock.Lock();
        CDataNode* n = tree.Search( "GUCEF%CORE%CDStoreCodecPluginManager", 
                                    '%'                                   ,
                                    false                                 );
        if ( n )
        {
                CString cmpstr( "CDStoreCodecPlugin" );
                CDStoreCodecPlugin* cp;
                CString path( "path" );
                const CDataNode* c;
                const CDataNode::TKeyValuePair* att; 
                CDataNode::const_iterator i = n->ConstBegin();
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
                                                cp = static_cast<CDStoreCodecPlugin*>(_codecs[ n ]);
                                                if ( att->second == cp->GetModulePath() )
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
                                                LoadCodecPlugin( att->second ); 
                                        }                                                
                                }       
                        }
                        ++i;
                }
        }                                     
        _datalock.Unlock();
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
