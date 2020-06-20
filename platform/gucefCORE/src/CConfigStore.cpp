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

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */ 

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"                  /* node for building data trees */
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"             /* abstract base for configurable objects */
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"       /* DStore codec registry */
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#include "CConfigStore.h"       /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CConfigStore::CConfigStore( void )
    : _codectype()
    , _configfile()
    , m_configureables()
    , m_newConfigureables()
    , m_isBusyLoadingConfig( false )
    , m_datalock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigStore::~CConfigStore()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetConfigFile( const CString& filepath )
{GUCEF_TRACE;

    MT::CScopeMutex scopeLock( m_datalock );
    _configfile = RelativePath( filepath );
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetConfigFile( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex scopeLock( m_datalock );
    return _configfile;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::Register( CIConfigurable* configobj )
{GUCEF_TRACE;

    MT::CScopeMutex scopeLock( m_datalock );

    if ( !m_isBusyLoadingConfig )
        m_configureables.insert( configobj );
    else
        m_newConfigureables.insert( configobj );
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::Unregister( CIConfigurable* configobj )
{GUCEF_TRACE;

    MT::CScopeMutex scopeLock( m_datalock );
    
    if ( m_isBusyLoadingConfig )
        return false;

    m_configureables.erase( configobj );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::SaveConfig( const CString& name ,
                          bool preserve       )
{GUCEF_TRACE;
        
        MT::CScopeMutex scopeLock( m_datalock );

        /*
         *      If the preserve flag is set then the old data tree
         *      will be loaded first to prevent any data loss.
         *      This basically allows you to add and/or modify data
         *      but not remove items from the tree unless you do so on purpose.
         */
        CDataNode rootnode( name );
        CDataNode oldtree;
        if ( preserve )
        {
                try
                {
                        CDStoreCodecRegistry::TDStoreCodecPtr codec = CCoreGlobal::Instance()->GetDStoreCodecRegistry().Lookup( _codectype.C_String() );
                        if ( codec->BuildDataTree( &oldtree    ,
                                                   _configfile ) )
                        {
                                rootnode.CopySubTree( oldtree );
                        }
                }
                catch ( CDStoreCodecRegistry::EUnregisteredName& )
                {
                        return false;
                }
        }

        TConfigurableSet::iterator i = m_configureables.begin();
        while ( i != m_configureables.end() )
        {
            if ( !(*i)->SaveConfig( rootnode ) )
            {
                return false;
            }
            ++i;
        }

        try
        {
                CDStoreCodecRegistry::TDStoreCodecPtr codec = CCoreGlobal::Instance()->GetDStoreCodecRegistry().Lookup( _codectype.C_String() );
                if ( codec->BuildDataTree( &rootnode    ,
                                           _configfile  ) )
                {
                        rootnode.CopySubTree( oldtree );
                        return true;
                }
        }
        catch ( CDStoreCodecRegistry::EUnregisteredName& )
        {
                return false;
        }

        return false;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::LoadConfig( CDataNode* loadedConfig )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "CConfigStore: Loading all config" );

    if ( !FileExists( _configfile ) )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Failed to load config because the config file does not exist: " + _configfile );
        return false;
    }

    MT::CScopeMutex scopeLock( m_datalock );

        if ( _codectype.Length() == 0 )
        {
            _codectype = Extract_File_Ext( _configfile.C_String() );
        }
        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CConfigStore: Will try to use codec \"" + _codectype + "\" for the config information in file " + _configfile );

        try
        {
            CDStoreCodecRegistry::TDStoreCodecPtr codec = CCoreGlobal::Instance()->GetDStoreCodecRegistry().Lookup( _codectype.C_String() );
            if ( 0 != codec )
            {
                CDataNode rootnode;
                if ( loadedConfig == GUCEF_NULL )
                    loadedConfig = &rootnode;
                if ( codec->BuildDataTree( loadedConfig ,
                                            _configfile ) )
                {
                        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CConfigStore: Used codec " + _codectype + " to successfully build a config data tree from file " + _configfile );

                        m_isBusyLoadingConfig = true;
                        
                        bool errorOccured = false;
                        TConfigurableSet::iterator i = m_configureables.begin();
                        while ( i != m_configureables.end() )
                        {
                            if ( !(*i)->LoadConfig( *loadedConfig ) )
                            {
                                errorOccured = true;
                                GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Loading of config failed for a configureable with type name \"" + (*i)->GetClassTypeName() + "\"" );
                            }
                            ++i;
                        }

                        // Loading of config can cause more configurables to register
                        // We handle those after dealing with the initial set
                        while ( !m_newConfigureables.empty() )
                        {
                            TConfigurableSet setCopy( m_newConfigureables );
                            i = setCopy.begin();
                            while ( i != setCopy.end() )
                            {
                                if ( !(*i)->LoadConfig( *loadedConfig ) )
                                {
                                    errorOccured = true;
                                    GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Loading of config failed for a late-addition configureable with type name \"" + (*i)->GetClassTypeName() + "\"" );
                                }
                                m_configureables.insert( (*i) );
                                m_newConfigureables.erase( (*i) );
                                ++i;
                            }
                        }

                        m_isBusyLoadingConfig = false;
                        
                        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CConfigStore: Successfully loaded all config using codec " + _codectype + " to successfully build a config data tree from file " + _configfile );
                        return !errorOccured;
                }
                else
                {
                    GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Failed to build a config data tree using codec " + _codectype + " from file " + _configfile );
                }
            }
            else
            {
                GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Failed to load config since no codec is available for representation \"" + _codectype + "\" in file " + _configfile );
            }
        }
        catch ( CDStoreCodecRegistry::EUnregisteredName& )
        {
            GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Failed to retrieve codec " + _codectype + " for the config information" );
        }
        return false;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetCodec( const CString& codectype )
{GUCEF_TRACE;

    MT::CScopeMutex scopeLock( m_datalock );
    _codectype = codectype;
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetCodec( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex scopeLock( m_datalock );
    return _codectype;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::Lock( void ) const
{GUCEF_TRACE;

    return m_datalock.Lock();
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::Unlock( void ) const
{GUCEF_TRACE;

    return m_datalock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
