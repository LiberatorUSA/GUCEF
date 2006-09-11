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
        : _codectype( "xml" )           ,
          _configfile( "GUCEFcfg.xml" )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigStore::CConfigStore( const CConfigStore& src )
{TRACE;
       
        /* dummy, should not be used */
}

/*-------------------------------------------------------------------------*/

CConfigStore::~CConfigStore()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CConfigStore& 
CConfigStore::operator=( const CConfigStore& src )
{TRACE;       
        /* dummy, should not be used */
        return *this;
}

/*-------------------------------------------------------------------------*/

CConfigStore*
CConfigStore::Instance( void )
{TRACE;              
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
{TRACE;        
        _datalock.Lock();
        delete _instance;
        _instance = NULL;
        _datalock.Unlock();
}        

/*-------------------------------------------------------------------------*/

void 
CConfigStore::SetConfigFile( const CString& filepath )
{TRACE;
        _datalock.Lock();
        _configfile = RelativePath( filepath );
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/
        
CString 
CConfigStore::GetConfigFile( void ) const
{TRACE;        
        _datalock.Lock();
        CString file( _configfile );
        _datalock.Unlock();
        return file;
}

/*-------------------------------------------------------------------------*/

void 
CConfigStore::Register( CIConfigurable* configobj )
{TRACE;
        _datalock.Lock();
        configobj->_configid = _configobjs.AddEntry( configobj );
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/
        
void 
CConfigStore::Unregister( CIConfigurable* configobj )
{TRACE;
        _datalock.Lock();
        _configobjs.SetEntry( configobj->_configid, NULL );
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

bool 
CConfigStore::SaveConfig( const CString& name ,
                          bool preserve       )
{TRACE;
        _datalock.Lock();
        
        /*
         *      If the preserve flag is set then the old data tree
         *      will be loaded first to prevent any data loss.
         *      This basicly allows you to add and/or modify data
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
{TRACE;        
        _datalock.Lock();        
        
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
{TRACE;        
        _codectype = codectype;
}

/*-------------------------------------------------------------------------*/
        
CString 
CConfigStore::GetCodec( void ) const
{TRACE;
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
