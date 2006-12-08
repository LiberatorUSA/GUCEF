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

#ifndef GUCEF_GUI_CGUICOMPONENT_H
#define GUCEF_GUI_CGUICOMPONENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "stdafx.h"
#include <string>
#include "XtraBase/XBGeneral/XBExceptionMacros.h"
#include "XtraBase/XBGeneral/CICloneable.h"
#include "XtraBase/XBGeneral/CObservingNotifier.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Abstract base class from which all form based GUI elements should be derived
 */
class CGUIForm : public XtraBase::CObservingNotifier ,
                        XtraBase::CICloneable
{
    public:

    CGUIForm( CGUIForm* parentForm = NULL );    

    virtual ~CGUIForm();

    CGUIForm* GetParentForm( void );

    const CGUIForm* GetParentForm( void ) const;

    virtual void SetPropertyValue( const std::string& formProperty  ,
                                   const std::string& propertyValue );

    virtual std::string GetPropertyValue( const std::string& formProperty ) const;

    virtual bool HasProperty( const bool const std::string& formProperty ) const;

    GUCEF_DEFINE_MSGEXCEPTION( XTRABASE_VOIDDEF, EUnknownProperty );

    protected:

    CGUIForm( const CGUIForm& src );

    CGUIForm& operator=( const CGUIForm& src );

    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( XtraBase::CNotifier* notifier           ,
                           const XtraBase::UInt32 eventid          ,
                           XtraBase::CICloneable* eventdata = NULL );

    private:

    CGUIForm* m_parentForm;
};

/*-------------------------------------------------------------------------*/

#endif /* APPGUIPROTO_CGUIFORM_H ? */