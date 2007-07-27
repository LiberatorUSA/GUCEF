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

#include <stdio.h>              /* standard I/O utils */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"        /* my verry own file handling utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* framework string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"   /* Additional utilities for manipulating string class objects */ 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#include "CPluginManager.h"    /* base class for all framework plugin managers */
#define GUCEF_CORE_CPLUGINMANAGER_H
#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"  /* main application class for GUCEF based applications */
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#include "CPluginControl.h"     /* definition of the class implemented here */

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

MT::CMutex CPluginControl::_mutex;
CPluginControl* CPluginControl::_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPluginControl*
CPluginControl::Instance( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        if ( !_instance )
        {
                _instance = new CPluginControl();
                CHECKMEM( _instance, sizeof(CPluginControl) );
                GUCEF_SYSTEM_LOG( 0, "GUCEF::CORE::CPluginControl Singleton created" );
        }
        _mutex.Unlock();
        GUCEF_END;
        return _instance; 
}

/*-------------------------------------------------------------------------*/

void 
CPluginControl::Deinstance( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();
        CHECKMEM( _instance, sizeof(CPluginControl) );
        delete _instance;
        _instance = NULL;
        GUCEF_SYSTEM_LOG( 0, "GUCEF::CORE::CPluginControl Singleton destroyed" );
        _mutex.Unlock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CPluginControl::~CPluginControl()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CPluginControl::CPluginControl( void )
        : _managers()  ,
          _plugindir()
{
        GUCEF_BEGIN;
        /*
         *      Set an initial plugin dir just in case we have problems getting
         *      a path later on.
         */
        _plugindir = RelativePath( "$CURWORKDIR$\\plugins" );        
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CPluginControl::CPluginControl( const CPluginControl& src )
{
        GUCEF_BEGIN;
        /* dummy, should never be used */
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CPluginControl& 
CPluginControl::operator=( const CPluginControl& src )
{
        GUCEF_BEGIN;
        /* dummy, should never be used */
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CPluginControl::SetPluginDir( const CString& path )
{
        GUCEF_BEGIN;        
        _mutex.Lock();
        
        CString abspath( RelativePath( path ) );
        _plugindir = abspath;       
        
        /*
         *      Notify all plugin managers
         */
        CPluginManager* pman;
        for ( Int32 i=0; i<=_managers.GetLast(); ++i )
        {
                pman = static_cast<CPluginManager*> (_managers[ i ]);
                if ( pman )
                {       
                        pman->OnSetPluginDir( abspath );
                }
        }
        _mutex.Unlock();
        GUCEF_END;        
}

/*-------------------------------------------------------------------------*/

CString
CPluginControl::GetPluginDir( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _plugindir;
}

/*-------------------------------------------------------------------------*/

void 
CPluginControl::LoadAll( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();        
        CPluginManager* pman;
        for ( Int32 i=0; i<=_managers.GetLast(); ++i )
        {
                pman = static_cast<CPluginManager*> (_managers[ i ]);
                if ( pman )
                {       
                        pman->LoadAll();
                }
        }
        _mutex.Unlock();  
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
        
void 
CPluginControl::UnloadAll( void )
{
        GUCEF_BEGIN;
        _mutex.Lock();        
        CPluginManager* pman;
        for ( Int32 i=0; i<=_managers.GetLast(); ++i )
        {
                pman = static_cast<CPluginManager*> (_managers[ i ]);
                if ( pman )
                {       
                        pman->UnloadAll();
                }
        }
        _mutex.Unlock();
        GUCEF_END;  
}

/*-------------------------------------------------------------------------*/

void 
CPluginControl::Register( CPluginManager* pman )
{
        GUCEF_BEGIN;
        _mutex.Lock();        
        pman->_managerid = _managers.AddEntry( pman );
        _mutex.Unlock(); 
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
        
void 
CPluginControl::Unregister( CPluginManager* pman )
{
        GUCEF_BEGIN;
        _mutex.Lock();        
        _managers.SetEntry( pman->_managerid , 
                            NULL             );
        _mutex.Unlock(); 
        GUCEF_END;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
