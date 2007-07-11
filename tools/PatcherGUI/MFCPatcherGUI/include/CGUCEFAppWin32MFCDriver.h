/*
 *  MFCPatcherGUI: GUI shell for the gucefPATCHER module
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef MFCWIN32_CGUCEFAPPWIN32MFCDRIVER_H
#define MFCWIN32_CGUCEFAPPWIN32MFCDRIVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <afxwin.h>

#ifndef GUCEF_CORE_CIGUCEFAPPLICATIONDRIVER_H
#include "CIGUCEFApplicationDriver.h"
#define GUCEF_CORE_CIGUCEFAPPLICATIONDRIVER_H
#endif /* GUCEF_CORE_CIGUCEFAPPLICATIONDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF::CORE::CGUCEFApplication;

/*-------------------------------------------------------------------------*/

class CGUCEFAppWin32MFCDriver : public CWnd                                  ,
                                public GUCEF::CORE::CIGUCEFApplicationDriver
{                          
    public:

    CGUCEFAppWin32MFCDriver( void );

    virtual ~CGUCEFAppWin32MFCDriver();

    bool Init( CWnd* pParentWnd );

    protected:

    virtual void OnSwitchUpdateMethod( const bool periodic );
    
    virtual void OnRequestNewMinimalUpdateFreq( const GUCEF::CORE::Float64 updateDeltaInMilliSecs );

    virtual void OnRequestNewUpdateCycle( void );

    protected:   

    //{{AFX_MSG(CGUCEFAppWin32MFCDriver)
    afx_msg void OnClose();
    afx_msg void OnTimer( UINT nIDEvent );
    afx_msg LRESULT OnGUCEFUpdateRequestMsg( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnStartUpdateTimer( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnStopUpdateTimer( WPARAM wParam, LPARAM lParam );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    private:

    CGUCEFAppWin32MFCDriver( const CGUCEFAppWin32MFCDriver& src );
    CGUCEFAppWin32MFCDriver& operator=( const CGUCEFAppWin32MFCDriver& src );

    private:

    UINT m_nTimer;
    GUCEF::CORE::Float64 m_frequency;
    GUCEF::CORE::CGUCEFApplication* m_appPtr;
    bool m_initialized;
    bool m_useTimer;
};

/*-------------------------------------------------------------------------*/

#endif /* MFCWIN32_CGUCEFAPPWIN32MFCDRIVER_H ? */