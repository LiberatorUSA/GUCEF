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

#ifndef GUCEF_CORE_COSWINDOW_H
#define GUCEF_CORE_COSWINDOW_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* module config macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

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
 *  Class with basic interface for an Operating System window
 *  Applies to graphical O/S's
 */
class GUCEF_CORE_PUBLIC_CPP COSWindow : public CObservingNotifier
{
    public:

    static const CEvent WindowCloseEvent;
    static const CEvent WindowDestroyEvent;
    static const CEvent WindowActivationEvent;
    static const CEvent WindowResizeEvent;
    static const CEvent WindowPaintEvent;

    static void RegisterEvents( void );

    public:

    // Build-in strategy pattern, there is only 1 right answer per graphical O/S
    static COSWindow* Create( void );
    static void Destroy( COSWindow* osWindow );
    
    public:

    COSWindow();

    virtual ~COSWindow();

    virtual bool WindowCreate( const CString& windowTitle ,
                               const Int32 xPosition      ,
                               const Int32 yPosition      ,
                               const UInt32 width         ,
                               const UInt32 height        ) = 0;    

    virtual void WindowDestroy( void ) = 0;

    virtual void Close( void ) = 0;

    virtual void SetText( const CString& text ) = 0;

    virtual CString GetText( void ) const = 0;

    virtual bool Show( void ) = 0;

    virtual bool Hide( void ) = 0;

    virtual bool Maximize( void ) = 0;

    virtual bool Minimize( void ) = 0;

    virtual void Repaint( void ) = 0;

    virtual void SendToForegound( void ) = 0;

    virtual void GrabFocus( void ) = 0;

    virtual int GetWidthOfClientAreaBorder( void ) const = 0;

    virtual bool GetWindowAreaPosition( int& x, int& y ) const = 0;

    virtual bool GetClientAreaPosition( int& x, int& y ) const = 0;

    virtual bool GetWindowArea( int& x, int& y, int& width, int& height ) const = 0;

    virtual bool GetWindowAreaRelativeToParentClientArea( Float32& x      ,
                                                          Float32& y      ,
                                                          Float32& width  ,
                                                          Float32& height ) const = 0;

    virtual bool GetClientArea( int& x, int& y, int& width, int& height ) const = 0;

    virtual void WindowAreaResize( int x, int y, int width, int height, bool repaint = true ) = 0;

    virtual void WindowAreaResize( int width, int height, bool repaint = true ) = 0;

    virtual void ClientAreaResize( int x, int y, int width, int height, bool repaint = true ) = 0;

    virtual void ClientAreaResize( int width, int height, bool repaint = true ) = 0;

    virtual void WindowAreaResizeRelativeToParentClientArea( Float32 x           ,
                                                             Float32 y           ,
                                                             Float32 width       ,
                                                             Float32 height      ,
                                                             bool repaint = true ) = 0;

    virtual void ClientAreaResizeRelativeToParentClientArea( Float32 x           ,
                                                             Float32 y           ,
                                                             Float32 width       ,
                                                             Float32 height      ,
                                                             bool repaint = true ) = 0;

    virtual CString GetProperty( const CString& propertyName ) const = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_COSWINDOW_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-04-2003 :
        - Initial implementation

---------------------------------------------------------------------------*/
