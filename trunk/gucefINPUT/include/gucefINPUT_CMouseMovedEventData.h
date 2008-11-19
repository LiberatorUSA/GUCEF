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

#ifndef GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H
#define GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_INPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEF_INPUT_MACROS_H
#endif /* GUCEF_INPUT_MACROS_H ? */

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

class GUCEF_INPUT_EXPORT_CPP CMouseMovedEventData : public CORE::CICloneable
{
    public:
    
    CMouseMovedEventData( const UInt32 deviceID ,
                          const UInt32 xPos     ,
                          const UInt32 yPos     ,
                          const UInt32 prevXPos ,
                          const UInt32 prevYPos );
    
    CMouseMovedEventData( const CMouseMovedEventData& src );
                        
    virtual ~CMouseMovedEventData();
    
    UInt32 GetDeviceID( void ) const;
        
    void GetPos( UInt32& xPos, UInt32& yPos ) const;
    
    void GetPrevPos( UInt32& xPos, UInt32& yPos ) const;
    
    UInt32 GetXPos( void ) const;
    
    UInt32 GetYPos( void ) const;

    UInt32 GetPrevXPos( void ) const;
    
    UInt32 GetPrevYPos( void ) const;
    
    virtual CORE::CICloneable* Clone( void ) const;
    
    private:
    
    CMouseMovedEventData( void );                                        /**< not implemented, can't use */
    CMouseMovedEventData& operator=( const CMouseMovedEventData& src );  /**< not implemented, no need */
    
    private:
    
    UInt32 m_deviceID;
    UInt32 m_xPos;
    UInt32 m_yPos;
    UInt32 m_prevXPos;
    UInt32 m_prevYPos;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CMOUSEMOVEDEVENTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 28-09-2007 :
        - Initial implementation

-----------------------------------------------------------------------------*/
