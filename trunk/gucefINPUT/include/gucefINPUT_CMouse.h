/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef GUCEF_INPUT_CMOUSE_H
#define GUCEF_INPUT_CMOUSE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_INPUT_CABSTRACTINPUTDEVICE_H
#include "gucefINPUT_CAbstractInputDevice.h"
#define GUCEF_INPUT_CABSTRACTINPUTDEVICE_H
#endif /* GUCEF_INPUT_CABSTRACTINPUTDEVICE_H ? */

#ifndef GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H
#include "gucefINPUT_CMouseButtonEventData.h"
#define GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H
#endif /* GUCEF_INPUT_CMOUSEBUTTONEVENTDATA_H ? */

#ifndef GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H
#include "gucefINPUT_CMouseMovedEventData.h"
#define GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H
#endif /* GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  In-software representation of a mouse hardware input device
 */
class GUCEF_INPUT_PUBLIC_CPP CMouse : public CAbstractInputDevice
{
    public:
    
    static const CORE::CEvent MouseButtonEvent;         /**< send when a mouse button is pressed or released, data = CMouseButtonEventData */
    static const CORE::CEvent MouseMovedEvent;          /**< send when the mouse moved, data = CMouseMovedEventData */
    static const CORE::CEvent MouseClickedEvent;        /**< send when a mouse button is clicked and released within a set interval */
    static const CORE::CEvent MouseDoubleClickedEvent;  /**< send when a mouse button is clicked and released twice within a set interval */
    static const CORE::CEvent MouseTrippleClickedEvent; /**< send when a mouse button is clicked and released three times within a set interval */
    
    static void RegisterEvents( void );

    static const char* DeviceType;                      /** string representing the type of device */
    
    public:
    
    typedef std::vector< bool > TButtonStates;
    
    bool IsButtonPressed( const UInt32 buttonIndex ) const;

    void GetMousePos( UInt32& xPos ,
                      UInt32& yPos ) const;

    void GetPrevMousePos( UInt32& xPos ,
                          UInt32& yPos ) const;
                      
    const TButtonStates& GetButtonStates( void ) const;
    
    virtual const char* GetDeviceType( void ) const;

    virtual bool IsDeviceType( const char* deviceType ) const;
    
    virtual const CString& GetClassTypeName( void ) const;

    private:
    friend class CInputController;
    
    CMouse( const UInt32 deviceID );
    
    virtual ~CMouse();
    
    void SetButtonState( const UInt32 buttonIndex , 
                         const bool pressedState );

    void SetMousePos( const UInt32 xPos ,
                      const UInt32 yPos );
                         
    void ResetMouseStates( void );
       
    private:
    
    CMouse( const CMouse& src );            /**< cannot be used */
    CMouse& operator=( const CMouse& src ); /**< cannot be used */
    
    private:
 
    UInt32 m_xPos;
    UInt32 m_yPos;
    UInt32 m_prevXPos;
    UInt32 m_prevYPos;    
    TButtonStates m_buttonStates;
    UInt32 m_deviceID;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CMOUSE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/
