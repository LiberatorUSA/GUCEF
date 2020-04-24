/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */
 
#ifndef GUCEF_GUI_CWINDOWMANAGER_H
#define GUCEF_GUI_CWINDOWMANAGER_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_GUI_CWINDOWMANAGERBACKEND_H
#include "gucefGUI_CWindowManagerBackend.h"
#define GUCEF_GUI_CWINDOWMANAGERBACKEND_H
#endif /* GUCEF_GUI_CWINDOWMANAGERBACKEND_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {
         
/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Central location were window manager backends can be obtained.
 */
class GUCEF_GUI_PUBLIC_CPP CWindowManager : public CORE::CObservingNotifier ,
                                            public CORE::CIConfigurable
{    
    public:

    typedef std::map< CORE::CString, TWindowManagerBackendPtr > TWindowManagerBackendMap;

    TWindowManagerBackendPtr GetBackend( const CORE::CString& typeName );

    void RegisterBackend( const CORE::CString& typeName    ,
                          TWindowManagerBackendPtr backend );

    void UnregisterBackend( const CORE::CString& typeName );

    void GetListOfAvailableBackends( TWindowManagerBackendMap& map );

    virtual bool SaveConfig( CORE::CDataNode& config ) const;

    virtual bool LoadConfig( const CORE::CDataNode& config );

    virtual const CString& GetClassTypeName( void ) const;

    private:
    friend class CGuiGlobal;

    CWindowManager( void );

    ~CWindowManager();

    private:

    CWindowManager( const CWindowManager& src );            /**< not implemented: singleton */
    CWindowManager& operator=( const CWindowManager& src ); /**< not implemented: singleton */

    private:

    TWindowManagerBackendMap m_backends;
    MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUI */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CWINDOWMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-04-2005 :
       - Initial version of this file.

---------------------------------------------------------------------------*/
