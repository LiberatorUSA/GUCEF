/*
 *  gucefDRN: GUCEF module providing RAD networking trough data replication
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

#ifndef GUCEF_DRN_CDRNDATAGROUPPROPERTIES_H
#define GUCEF_DRN_CDRNDATAGROUPPROPERTIES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_DRN_MACROS_H
#include "gucefDRN_macros.h"
#define GUCEF_DRN_MACROS_H
#endif /* GUCEF_DRN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace DRN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Property set for a DRN data group.
 */
class GUCEF_DRN_EXPORT_CPP CDRNDataGroupProperties
{
    public:
    
    CDRNDataGroupProperties( void );
    
    CDRNDataGroupProperties( const CDRNDataGroupProperties& src );
    
    virtual ~CDRNDataGroupProperties();
    
    CDRNDataGroupProperties& operator=( const CDRNDataGroupProperties& src );
    
    /**
     *  Default is false
     */
    void SetAcceptNewPeerItems( const bool acceptNewPeerItems );
    
    bool GetAcceptNewPeerItems( void ) const;    
    
    /**
     *  Default is false
     */
    void SetEmitEntireGroupOnChange( const bool emitEntireGroup );
    
    bool GetEmitEntireGroupOnChange( void ) const;
    
    /**
     *  Default is true
     */
    void SetAcceptNewStreamerItems( const bool acceptNewSItems );
    
    bool GetAcceptNewStreamerItems( void ) const;
    
    /**
     *  Default is true
     */
    void SetAcceptUpdatesFromStreamers( const bool acceptStreamerUpdates );
    
    bool GetAcceptUpdatesFromStreamers( void ) const;

    private:
    
    bool m_emitAsGroup;
    bool m_acceptNewStreamerItems;
    bool m_acceptStreamerUpdates;
    bool m_acceptNewPeerItems;
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DRN_CDRNDATAGROUPPROPERTIES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
