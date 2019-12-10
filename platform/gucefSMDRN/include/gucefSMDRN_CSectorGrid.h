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

#ifndef GUCEF_SMDRN_CSECTORGRID_H
#define GUCEF_SMDRN_CSECTORGRID_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_SMDRN_CSECTOR_H
#include "gucefSMDRN_CSector.h"
#define GUCEF_SMDRN_CSECTOR_H
#endif /* GUCEF_SMDRN_CSECTOR_H ? */

#ifndef GUCEF_SMDRN_CSECTORPOSITION_H
#include "gucefSMDRN_CSectorPosition.h"
#define GUCEF_SMDRN_CSECTORPOSITION_H
#endif /* GUCEF_SMDRN_CSECTORPOSITION_H ? */

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

class GUCEF_SMDRN_EXPORT_CPP CSectorGrid
{
    public:
    
    CSectorGrid( void );
    
    CSectorGrid( const UInt16 gridWidth  ,
                 const UInt16 gridHeight ,
                 const UInt16 gridDepth  );
                 
    ~CSectorGrid();
    
    void SetGridSize( const UInt16 gridWidth  ,
                      const UInt16 gridHeight ,
                      const UInt16 gridDepth  );
                      
    CSector* GetSector( const UInt16 x ,
                        const UInt16 y ,
                        const UInt16 z );
                        
    CSector* GetSector( const CSectorPosition& position );
                        
    void MoveObjectTo( const CSectorPosition& position );
    
    void MoveObject( void );
 
    private:

    CSectorGrid( const CSectorGrid& src );    
    CSectorGrid& operator=( const CSectorGrid& src );
    
    

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace SMDRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_SMDRN_CSECTORGRID_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/