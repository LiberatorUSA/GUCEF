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

#ifndef GUCEF_CORE_CMSWIN32WINDOW_H
#define GUCEF_CORE_CMSWIN32WINDOW_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* module config macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifdef GUCEF_MSWIN_BUILD

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class with basic utilities for creating a Win32 window
 */
class GUCEF_CORE_EXPORT_CPP CMsWin32Window
{
    public:
    
    CMsWin32Window();

    virtual ~CMsWin32Window();
    
    HWND GetHwnd( void ) const;
    
    virtual const CString& GetClassTypeName( void ) const;

    protected:

    virtual bool WindowCreate( const CString& windowClassName ,
                               const CString& windowTitle     );
    
    static bool RegisterWindowClass( const CString& windowClassName );
    
    virtual LRESULT WindowProc( const HWND hWnd     ,
                                const UINT nMsg     ,
                                const WPARAM wParam ,
                                const LPARAM lParam );

    void SetHwnd( HWND hwnd );

    private:

    CMsWin32Window( const CMsWin32Window& src );       /**< cannot be copied */
    CMsWin32Window& operator=( const CMsWin32Window ); /**< cannot be copied */
    
    static LRESULT WndProc( HWND hwnd     , 
                            UINT msg      ,
                            WPARAM wParam ,
                            LPARAM lParam );
    
    private:
    
    HWND m_hwnd;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */

#endif /* GUCEF_CORE_CMSWIN32WINDOW_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-04-2003 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
