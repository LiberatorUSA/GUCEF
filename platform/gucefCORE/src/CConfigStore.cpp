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

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

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

#ifndef GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#include "gucefCORE_CGloballyConfigurable.h"
#define GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#endif /* GUCEF_CORE_CGLOBALLYCONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"       /* DStore codec registry */
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"        
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CConfigStore::GlobalBootstrapConfigLoadStartingEvent = "GUCEF::CORE::CConfigStore::GlobalBootstrapConfigLoadStartingEvent";
const CEvent CConfigStore::GlobalBootstrapConfigLoadCompletedEvent = "GUCEF::CORE::CConfigStore::GlobalBootstrapConfigLoadCompletedEvent";
const CEvent CConfigStore::GlobalBootstrapConfigLoadFailedEvent = "GUCEF::CORE::CConfigStore::GlobalBootstrapConfigLoadFailedEvent";
const CEvent CConfigStore::GlobalConfigLoadStartingEvent = "GUCEF::CORE::CConfigStore::GlobalConfigLoadStartingEvent";
const CEvent CConfigStore::GlobalConfigLoadCompletedEvent = "GUCEF::CORE::CConfigStore::GlobalConfigLoadCompletedEvent";
const CEvent CConfigStore::GlobalConfigLoadFailedEvent = "GUCEF::CORE::CConfigStore::GlobalConfigLoadFailedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CConfigStore::RegisterEvents( void )
{GUCEF_TRACE;

    GlobalBootstrapConfigLoadStartingEvent.Initialize();
    GlobalBootstrapConfigLoadCompletedEvent.Initialize();
    GlobalBootstrapConfigLoadFailedEvent.Initialize();
    GlobalConfigLoadStartingEvent.Initialize();
    GlobalConfigLoadCompletedEvent.Initialize();
    GlobalConfigLoadFailedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CConfigStore::CConfigStore( void )
    : CTSGNotifier()
    , _codectype()
    , m_bootstrapConfigFile()
    , m_consolidatedConfigFile()
    , m_configfiles()
    , m_configureables()
    , m_newConfigureables()
    , m_isBusyLoadingConfig( false )
    , m_globalConfigIsLoaded( false )
    , m_globalBootstrapConfigIsLoaded( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigStore::~CConfigStore()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetBootstrapConfigFile( const CString& filepath )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_bootstrapConfigFile = filepath;
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetBootstrapConfigFile( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    return m_bootstrapConfigFile;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetConfigFile( const CString& filepath ,
                             bool isAddative         )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_consolidatedConfigFile = filepath;
    if ( !isAddative )
        m_configfiles.clear();
    m_configfiles.push_back( RelativePath( filepath ) );
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetConfigFile( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    if ( !m_consolidatedConfigFile.IsNULLOrEmpty() )
        return m_consolidatedConfigFile;
    if ( m_configfiles.empty() )
        return CString::Empty;
    return m_configfiles.back();
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetConsolidatedConfigFile( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    return m_consolidatedConfigFile;
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetConsolidatedConfigFileCodecType( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    CString codecType = ExtractFileExtention( m_consolidatedConfigFile );
    if ( codecType.IsNULLOrEmpty() )
        codecType = _codectype;
    return codecType;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetConfigFiles( const CString::StringVector& filepaths ,
                              bool isAddative                        )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    if ( !isAddative )
        m_configfiles.clear();

    CString::StringVector::const_iterator i = filepaths.begin();
    while ( i != filepaths.end() )
    {
        m_configfiles.push_back( RelativePath( (*i) ) );
        ++i;
    }
    
    if ( m_consolidatedConfigFile.IsNULLOrEmpty() && !m_configfiles.empty() )
    {       
        // Generated a default consolidated config file name
        CString firstCfg = (*m_configfiles.begin());
        m_consolidatedConfigFile = StripFileExtention( firstCfg ) + "_consolidated." + ExtractFileExtention( firstCfg );
    }
}

/*-------------------------------------------------------------------------*/

CString::StringVector
CConfigStore::GetConfigFiles( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    return m_configfiles;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::Register( CGloballyConfigurable* configobj )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( !m_isBusyLoadingConfig )
        m_configureables.insert( configobj );
    else
        m_newConfigureables.insert( configobj );
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::Unregister( CGloballyConfigurable* configobj )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    if ( m_isBusyLoadingConfig )
        return false;

    m_configureables.erase( configobj );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::SaveConsolidatedConfig( const CString& name ,
                                      bool preserve       )
{GUCEF_TRACE;
        
    MT::CObjectScopeLock lock( this );

    CString codecType = GetConsolidatedConfigFileCodecType();
    
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
            CDStoreCodecRegistry::TDStoreCodecPtr codec = CCoreGlobal::Instance()->GetDStoreCodecRegistry().Lookup( codecType );
            if ( codec->BuildDataTree( &oldtree                  ,
                                       m_consolidatedConfigFile  ) )
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
        CDStoreCodecRegistry::TDStoreCodecPtr codec = CCoreGlobal::Instance()->GetDStoreCodecRegistry().Lookup( codecType );
        if ( codec->BuildDataTree( &rootnode                 ,
                                   m_consolidatedConfigFile  ) )
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
CConfigStore::SaveConsolidatedConfig( const CDataNode& newCfg )
{GUCEF_TRACE;

    try
    {
        CString codecType = GetConsolidatedConfigFileCodecType();
        CString consolidatedConfigFilePath = GetConsolidatedConfigFile();
        
        CDStoreCodecRegistry::TDStoreCodecPtr codec;
        if ( CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( codecType, codec, false ) && !codec.IsNULL() )
        {
            if ( codec->StoreDataTree( &newCfg                     ,
                                       consolidatedConfigFilePath  ) )
            {
                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ConfigStore:SaveConsolidatedConfig: Wrote consolidated config using codec \"" + codecType + "\" to: " + consolidatedConfigFilePath );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ConfigStore:SaveConsolidatedConfig: Failed to write consolidated config using codec \"" + codecType + "\" to: " + consolidatedConfigFilePath );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ConfigStore:SaveConsolidatedConfig: Failed obtain codec \"" + codecType + "\"" );
        }
    }
    catch ( std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( LOGLEVEL_NORMAL, CString( "ConfigStore:SaveConsolidatedConfig: Exception occured: " ) + e.what() );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::ApplyConsolidatedConfig( const CDataNode& newCfg )
{GUCEF_TRACE;
    
    // Before we do anything drastic make sure our logs are flushed to a known point to help diagnostics
    CORE::CCoreGlobal::Instance()->GetLogManager().FlushLogs();

    MT::CObjectScopeLock lock( this );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ConfigStore:ApplyConsolidatedConfig: commencing" );
    
    bool success = true;
    try
    {
        m_globalBootstrapConfigIsLoaded = false;
        m_globalConfigIsLoaded = false;
        if ( !NotifyObservers( GlobalConfigLoadStartingEvent ) ) 
            return false;
        
        {
            success = ApplyConfigData( newCfg, false, !m_bootstrapConfigFile.IsNULLOrEmpty() );
        }

        if ( success )
        {
            m_globalBootstrapConfigIsLoaded = true;
            m_globalConfigIsLoaded = true;
            NotifyObservers( GlobalConfigLoadCompletedEvent );
        }
        else
            NotifyObservers( GlobalConfigLoadFailedEvent );
    
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ConfigStore:ApplyConsolidatedConfig: completed" );

        lock.EarlyUnlock();

        // After anything drastic make sure our logs are flushed to a known point to help diagnostics        
        CORE::CCoreGlobal::Instance()->GetLogManager().FlushLogs();
    }
    catch ( std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( LOGLEVEL_NORMAL, CString( "ConfigStore:SaveConsolidatedConfig: Exception occured: " ) + e.what() );
        success = false;
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::LoadConfig( CDataNode* loadedConfig )
{GUCEF_TRACE;

    CDataNode configTmp;
    CDataNode& config = GUCEF_NULL == loadedConfig ? configTmp : *loadedConfig; 
    bool totalSuccess = true;

    {
        MT::CObjectScopeLock lock( this );
        
        // Do we have a bootstrap log? If so apply that first
        if ( !m_bootstrapConfigFile.IsNULLOrEmpty() )
        {
            m_globalConfigIsLoaded = false;
            m_globalBootstrapConfigIsLoaded = false;
            
            if ( !NotifyObservers( GlobalBootstrapConfigLoadStartingEvent ) ) 
                return false;

            totalSuccess = LoadConfigData( m_bootstrapConfigFile, config );
    
            // IF we have a bootstrap config it MUST load successfully
            if ( !totalSuccess )
            {
                NotifyObservers( GlobalBootstrapConfigLoadFailedEvent );
                return false;
            }

            totalSuccess = ApplyConfigData( config, true, true );

            if ( totalSuccess )
            {
                m_globalBootstrapConfigIsLoaded = true;
                NotifyObservers( GlobalBootstrapConfigLoadCompletedEvent );
            }
            else
            {
                NotifyObservers( GlobalBootstrapConfigLoadFailedEvent );
            }
        }
        else
        {
            m_globalBootstrapConfigIsLoaded = true;
        }
    }
    {
        MT::CObjectScopeLock lock( this );
    
        if ( !NotifyObservers( GlobalConfigLoadStartingEvent ) ) 
            return false;

        // Load and overlay configs in the order given
        CString::StringVector::const_iterator i = m_configfiles.begin();
        while ( i != m_configfiles.end() )
        {
            totalSuccess = LoadConfigData( (*i), config ) && totalSuccess;
            ++i;
        }
    
        // If multiple config files are given they MUST all load successfully
        if ( !totalSuccess )
        {
            NotifyObservers( GlobalConfigLoadFailedEvent );
            return false;
        }

        // Now we actually apply the config to globally linked configurables
        totalSuccess = ApplyConfigData( config, false, !m_bootstrapConfigFile.IsNULLOrEmpty() );

        if ( totalSuccess )
        {
            m_globalConfigIsLoaded = true;
            NotifyObservers( GlobalConfigLoadCompletedEvent );
        }
        else
            NotifyObservers( GlobalConfigLoadFailedEvent );
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::LoadConfigData( const CString& cfgFilepath , 
                              CDataNode& loadedConfig    )
{GUCEF_TRACE;
                          
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ConfigStore:LoadConfigData: Will attempt to load all config from file: " + cfgFilepath );

    if ( !FileExists( cfgFilepath ) )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ConfigStore:LoadConfigData: Failed to load config because the config file does not exist: " + cfgFilepath );
        return false;
    }

    CORE::CString codectype = Extract_File_Ext( cfgFilepath.C_String() );
    if ( codectype.IsNULLOrEmpty() )
    {
        codectype = _codectype;
    }
    GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "ConfigStore:LoadConfigData: Will try to use codec \"" + codectype + "\" for the config information in file " + cfgFilepath );

    try
    {
        CDStoreCodecRegistry::TDStoreCodecPtr codec;
        if ( CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( codectype, codec, false ) && !codec.IsNULL() )
        {
            if ( codec->BuildDataTree( &loadedConfig ,
                                       cfgFilepath   ) )
            {
                    GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "ConfigStore:LoadConfigData: Used codec " + codectype + " to successfully build a config data tree from file " + cfgFilepath );
                    return true;
            }
            else
            {
                GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ConfigStore:LoadConfigData: Failed to build a config data tree using codec " + codectype + " from file " + cfgFilepath );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ConfigStore:LoadConfigData: Failed to load config since no codec is available for representation \"" + codectype + "\" in file " + cfgFilepath );
        }
    }
    catch ( CDStoreCodecRegistry::EUnregisteredName& )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ConfigStore:LoadConfigData: Failed to retrieve codec " + codectype + " for the config information" );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::ApplyConfigData( const CDataNode& loadedConfig , 
                               bool isBootstrap              ,
                               bool hasBootstrap             )
{GUCEF_TRACE;

    m_isBusyLoadingConfig = true;
                        
    bool errorOccured = false;
    TConfigurableSet::iterator i = m_configureables.begin();
    while ( i != m_configureables.end() )
    {
        if ( !hasBootstrap || ( isBootstrap == (*i)->IsIncludedInGlobalBootstrapConfigLoad() ) )
        {
            if ( !(*i)->LoadConfig( loadedConfig ) )
            {
                errorOccured = true;
                GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ConfigStore:ApplyConfigData: Loading of config failed for a configureable with type name \"" + (*i)->GetClassTypeName() + "\"" );
            }
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
            if ( !hasBootstrap || ( isBootstrap == (*i)->IsIncludedInGlobalBootstrapConfigLoad() ) )
            {
                if ( !(*i)->LoadConfig( loadedConfig ) )
                {
                    errorOccured = true;
                    GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ConfigStore:ApplyConfigData: Loading of config failed for a late-addition configureable with type name \"" + (*i)->GetClassTypeName() + "\"" );
                }            
            }

            m_configureables.insert( (*i) );
            m_newConfigureables.erase( (*i) );
            ++i;
        }
    }

    m_isBusyLoadingConfig = false;
                        
    if ( !errorOccured )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "ConfigStore:ApplyConfigData: Successfully loaded all config for all globally registered configurables" );
        return true;
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ConfigStore:ApplyConfigData: Failed loading config for all globally registered configurables" );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool 
CConfigStore::IsGlobalBootstrapConfigLoadInProgress( void ) const
{GUCEF_TRACE;

    return m_isBusyLoadingConfig && !m_globalBootstrapConfigIsLoaded;
}

/*-------------------------------------------------------------------------*/

bool 
CConfigStore::IsGlobalConfigLoadInProgress( void ) const
{GUCEF_TRACE;

    return m_isBusyLoadingConfig && !m_globalConfigIsLoaded;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::IsGlobalConfigLoaded( void ) const
{GUCEF_TRACE;

    return m_globalConfigIsLoaded;
}

/*-------------------------------------------------------------------------*/
    
bool 
CConfigStore::IsGlobalBootstrapConfigIsLoaded( void ) const
{GUCEF_TRACE;

    return m_globalBootstrapConfigIsLoaded;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetCodec( const CString& codectype )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    _codectype = codectype;
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetCodec( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    return _codectype;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
