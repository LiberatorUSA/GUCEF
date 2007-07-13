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
MT::CMutex CConfigStore::_datalock;

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

CConfigStore::CConfigStore( const CConfigStore& src )
{GUCEF_TRACE;
       
        /* dummy, should not be used */
}

/*-------------------------------------------------------------------------*/

CConfigStore::~CConfigStore()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigStore& 
CConfigStore::operator=( const CConfigStore& src )
{GUCEF_TRACE;       
        /* dummy, should not be used */
        return *this;
}

/*-------------------------------------------------------------------------*/

CConfigStore*
CConfigStore::Instance( void )
{GUCEF_TRACE;              
        _datalock.Lock();
        if ( !_instance )
        {
                _instance = new CConfigStore();
                CHECKMEM( _instance, sizeof(CConfigStore) );
        }
        _datalock.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void
CConfigStore::Deinstance( void )
{GUCEF_TRACE;        
        _datalock.Lock();
        delete _instance;
        _instance = NULL;
        _datalock.Unlock();
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
        configobj->_configid = _configobjs.AddEntry( configobj );
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/
        
void 
CConfigStore::Unregister( CIConfigurable* configobj )
{GUCEF_TRACE;
        _datalock.Lock();
        _configobjs.SetEntry( configobj->_configid, NULL );
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
                        CDStoreCodecRegistry::TDStoreCodecPtr codec = CDStoreCodecRegistry::Instance()->Lookup( _codectype.C_String() );
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
        
        UInt32 count = _configobjs.GetCount();
        CIConfigurable* c;
        for ( UInt32 i=0; i<count; ++i )
        {
                c = static_cast<CIConfigurable*>( _configobjs[ i ] );
                if ( c )
                {
                        if ( !c->SaveConfig( rootnode ) )
                        {
                                _datalock.Unlock();
                                return false;                
                        }
                }
        }        
        
        try
        {
                CDStoreCodecRegistry::TDStoreCodecPtr codec = CDStoreCodecRegistry::Instance()->Lookup( _codectype.C_String() );
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
        _datalock.Lock();        
        
        if ( _codectype.Length() == 0 )
        {
            _codectype = Extract_File_Ext( _configfile.C_String() );
        }
        
        try 
        {
                CDStoreCodecRegistry::TDStoreCodecPtr codec = CDStoreCodecRegistry::Instance()->Lookup( _codectype.C_String() );
                CDataNode rootnode;
                if ( codec->BuildDataTree( &rootnode   ,
                                           _configfile ) )
                {
                        UInt32 count = _configobjs.GetCount();
                        CIConfigurable* c;
                        for ( UInt32 i=0; i<count; ++i )
                        {
                                c = static_cast<CIConfigurable*>( _configobjs[ i ] );
                                if ( c )
                                {
                                        if ( !c->LoadConfig( rootnode ) )
                                        {
                                                _datalock.Unlock();
                                                return false;                
                                        }
                                }
                        }
                        _datalock.Unlock();
                        return true;
                }                      
        }                
        catch ( CDStoreCodecRegistry::EUnregisteredName& )
        {
        
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
