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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CConfigStore* CConfigStore::_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CConfigStore::CConfigStore( void )
        : _codectype()  ,
          _configfile()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigStore::~CConfigStore()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigStore*
CConfigStore::Instance( void )
{GUCEF_TRACE;

    if ( NULL == _instance )
    {
        _instance = new CConfigStore();
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CConfigStore Singleton created" );
    }
    return _instance;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::Deinstance( void )
{GUCEF_TRACE;

    delete _instance;
    _instance = NULL;
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CConfigStore Singleton destroyed" );
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetConfigFile( const CString& filepath )
{GUCEF_TRACE;
        _datalock.Lock();
        _configfile = RelativePath( filepath );
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetConfigFile( void ) const
{GUCEF_TRACE;

    _datalock.Lock();
    CString file( _configfile );
    _datalock.Unlock();
    return file;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::Register( CIConfigurable* configobj )
{GUCEF_TRACE;

    _datalock.Lock();
    _configobjs.insert( configobj );
    _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::Unregister( CIConfigurable* configobj )
{GUCEF_TRACE;

    _datalock.Lock();
    _configobjs.erase( configobj );
    _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::SaveConfig( const CString& name ,
                          bool preserve       )
{GUCEF_TRACE;
        _datalock.Lock();

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
                        _datalock.Unlock();
                        return false;
                }
        }

        TConfigurableSet::iterator i = _configobjs.begin();
        while ( i != _configobjs.end() )
        {
            if ( !(*i)->SaveConfig( rootnode ) )
            {
                _datalock.Unlock();
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
                _datalock.Unlock();
                return false;
        }

        _datalock.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CConfigStore::LoadConfig( void )
{GUCEF_TRACE;

        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "CConfigStore: Loading all config" );

        if ( !FileExists( _configfile ) )
        {
            GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Failed to load config because the config file does not exist: " + _configfile );
            return false;
        }

        _datalock.Lock();

        if ( _codectype.Length() == 0 )
        {
            _codectype = Extract_File_Ext( _configfile.C_String() );
        }
        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CConfigStore: Will try to use codec " + _codectype + " for the config information in file " + _configfile );

        try
        {
                CDStoreCodecRegistry::TDStoreCodecPtr codec = CCoreGlobal::Instance()->GetDStoreCodecRegistry().Lookup( _codectype.C_String() );
                CDataNode rootnode;
                if ( codec->BuildDataTree( &rootnode   ,
                                           _configfile ) )
                {
                        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CConfigStore: Used codec " + _codectype + " to successfully build a config data tree from file " + _configfile );

                        TConfigurableSet::iterator i = _configobjs.begin();
                        while ( i != _configobjs.end() )
                        {
                            if ( !(*i)->LoadConfig( rootnode ) )
                            {
                                    _datalock.Unlock();
                                    return false;
                            }
                            ++i;
                        }
                        _datalock.Unlock();

                        GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "CConfigStore: Successfully loaded all config using codec " + _codectype + " to successfully build a config data tree from file " + _configfile );
                        return true;
                }
                else
                {
                    GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Failed to build a config data tree using codec " + _codectype + " from file " + _configfile );
                }
        }
        catch ( CDStoreCodecRegistry::EUnregisteredName& )
        {
            GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CConfigStore: Failed to retrieve codec " + _codectype + " for the config information" );
        }
        _datalock.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::SetCodec( const CString& codectype )
{GUCEF_TRACE;
        _codectype = codectype;
}

/*-------------------------------------------------------------------------*/

CString
CConfigStore::GetCodec( void ) const
{GUCEF_TRACE;
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
