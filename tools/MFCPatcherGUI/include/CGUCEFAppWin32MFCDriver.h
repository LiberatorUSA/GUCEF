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

    virtual void OnSwitchUpdateMethod( const bool periodic ) = 0;
    
    virtual void OnRequestNewMinimalUpdateFreq( const GUCEF::CORE::UInt32 frequenty );

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
    GUCEF::CORE::UInt32 m_frequency;
    GUCEF::CORE::CGUCEFApplication* m_appPtr;
    bool m_initialized;
};

/*-------------------------------------------------------------------------*/

#endif /* MFCWIN32_CGUCEFAPPWIN32MFCDRIVER_H ? */