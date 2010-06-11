/*
 *  gucefSMDRN: GUCEF module providing sector managed DRN
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_SMDRN_CSECTORPOSITION_H
#define GUCEF_SMDRN_CSECTORPOSITION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_SMDRN_MACROS_H
#include "gucefSMDRN_macros.h"         /* often used gucefSMDRN macros */
#define GUCEF_SMDRN_MACROS_H
#endif /* GUCEF_SMDRN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace SMDRN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class that stores a sector position which is a 
 *  (x,y,z) coordinate within a sector and the coordinates of the sector itself.
 *  This class ensures position values are always clamped within the accepted range 
 *  limiting the number of location in the code where range checking needs to be performed.
 *  It also allows you to perform easy arithmatic operations without having to worry about 
 *  the fact we are dealing with multiple coordinate systems within a larger grid. 
 */
class GUCEF_SMDRN_EXPORT_CPP CSectorPosition
{
    public
    
    const UInt32 MaxAxisValue;
    
    public:

    CSectorPosition( CSectorGrid* sectorGrid );
    
    CSectorPosition( CSectorGrid* sectorGrid ,
                     const UInt32 xValue     , 
                     const UInt32 yValue     ,
                     const UInt32 zValue     ,
                     const UInt16 sectorX    ,
                     const UInt16 sectorY    ,
                     const UInt16 sectorZ    );
    
    CSectorPosition( const CSectorPosition& src );
        
    CSectorPosition& operator=( const CSectorPosition& src );

    ~CSectorPosition();
    
    UInt32 GetX( void ) const;
    
    UInt32 GetY( void ) const;
    
    UInt32 GetZ( void ) const;

    void Get( UInt32& xValue , 
              UInt32& yValue ,
              UInt32& zValue ) const;
                  
    void SetX( const UInt32 value );
    
    void SetY( const UInt32 value );
    
    void SetZ( const UInt32 value );
    
    void Set( const UInt32 xValue , 
              const UInt32 yValue ,
              const UInt32 zValue );
  
    static UInt32 ClampPositionAxisValue( const UInt32 value );
    
    void SetSectorLocation( const UInt16 x ,
                            const UInt16 y ,
                            const UInt16 z );
                    
    void GetSectorLocation( UInt16& xValue , 
                            UInt16& yValue ,
                            UInt16& zValue ) const;
    
    private:

    UInt32 m_positionX;
    UInt32 m_positionY;
    UInt32 m_positionZ;
    UInt16 m_sectorX;
    UInt16 m_sectorY;
    UInt16 m_sectorZ;
    CSectorGrid* m_sectorGrid;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace SMDRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_SMDRN_CSECTORPOSITION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/