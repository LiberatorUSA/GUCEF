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

#include <stdio.h>              /* standard I/O utils */

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

#ifndef GUCEF_CORE_CIPLUGINMANAGER_H
#include "CIPluginManager.h"    /* interface base class for all framework plugin managers */
#define GUCEF_CORE_CIPLUGINMANAGER_H
#endif /* GUCEF_CORE_CIPLUGINMANAGER_H ? */

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
        CIPluginManager* pman;
        for ( Int32 i=0; i<=_managers.GetLast(); ++i )
        {
                pman = static_cast<CIPluginManager*> (_managers[ i ]);
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
        CIPluginManager* pman;
        for ( Int32 i=0; i<=_managers.GetLast(); ++i )
        {
                pman = static_cast<CIPluginManager*> (_managers[ i ]);
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
        CIPluginManager* pman;
        for ( Int32 i=0; i<=_managers.GetLast(); ++i )
        {
                pman = static_cast<CIPluginManager*> (_managers[ i ]);
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
CPluginControl::Register( CIPluginManager* pman )
{
        GUCEF_BEGIN;
        _mutex.Lock();        
        pman->_managerid = _managers.AddEntry( pman );
        _mutex.Unlock(); 
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
        
void 
CPluginControl::Unregister( CIPluginManager* pman )
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
