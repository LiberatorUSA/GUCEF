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

#ifndef GUCEF_INPUT_CINPUTDEVICE_H
#define GUCEF_INPUT_CINPUTDEVICE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

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
 *  In-software representation of a hardware input device
 *  This representation will typically be used for things like joysticks
 */
class GUCEF_INPUT_PUBLIC_CPP CInputDevice : public CORE::CNotifier
{
    public:
    
    static const CORE::CEvent BoolStateChangedEvent; /**< send when a device boolean state changes, data = CInputDeviceBoolStateChangedEventData */
    static const CORE::CEvent VarStateChangedEvent;  /**< send when a device variable state changes, data = CInputDeviceVarStateChangedEventData */
    
    static void RegisterEvents( void );
    
    public:
    
    typedef std::map< UInt32, bool > TBooleanStates;
    
    bool GetBoolState( const UInt32 index ) const;
    
    virtual const CString& GetType( void ) const;

    private:
    friend class CInputController;
    
    CInputDevice( void );
    
    virtual ~CInputDevice();

       
    private:
    
    CInputDevice( const CInputDevice& src );            /**< cannot be used */
    CInputDevice& operator=( const CInputDevice& src ); /**< cannot be used */
    
    private:
 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CINPUTDEVICE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/
