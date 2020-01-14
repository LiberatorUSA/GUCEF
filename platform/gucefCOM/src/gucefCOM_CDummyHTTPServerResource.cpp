/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCOM_CDummyHTTPServerResource.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDummyHTTPServerResource::CDummyHTTPServerResource( void )
    : CDefaultHTTPServerResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDummyHTTPServerResource::CDummyHTTPServerResource( const CDummyHTTPServerResource& src )
    : CDefaultHTTPServerResource( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDummyHTTPServerResource::~CDummyHTTPServerResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDummyHTTPServerResource&
CDummyHTTPServerResource::operator=( const CDummyHTTPServerResource& src )
{GUCEF_TRACE;

    CDefaultHTTPServerResource::operator=( src );
    return *this;
}

/*-------------------------------------------------------------------------*/

CString 
CDummyHTTPServerResource::GetBestMatchedSerializationRepresentation( const CDummyHTTPServerResource::TStringVector& representations )
{GUCEF_TRACE;

    if ( !representations.empty() )
    {
        // For the dummy resource we always want to fake success
        // Same thing for the MIME types, we always match the first ask
        return *representations.begin();
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

CString 
CDummyHTTPServerResource::GetBestSupportedDeserializationRepresentation( const TStringVector& representations )
{GUCEF_TRACE;

    if ( !representations.empty() )
    {
        // For the dummy resource we always want to fake success
        // Same thing for the MIME types, we always match the first ask
        return *representations.begin();
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

CDummyHTTPServerResource::TCreateState 
CDummyHTTPServerResource::CreateResource( const CString& transactionID                  ,
                                            const CORE::CDynamicBuffer& inputBuffer       ,
                                            const CString& representation                 ,
                                            THTTPServerResourcePtr& resourceOutput        ,
                                            TStringVector& supportedRepresentationsOutput )
{GUCEF_TRACE;

    // For the dummy resource everything always fake succeeds
    return TCreateState::CREATESTATE_CREATED;
}

/*-------------------------------------------------------------------------*/
    
bool 
CDummyHTTPServerResource::DeleteResource()
{GUCEF_TRACE;

    // For the dummy resource everything always fake succeeds
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CDummyHTTPServerResource::Serialize( CORE::CDynamicBuffer& outputBuffer ,
                                       const CString& representation      )
{GUCEF_TRACE;

    // For the dummy resource everything always fake succeeds
    return true;
}

/*-------------------------------------------------------------------------*/

CDummyHTTPServerResource::TDeserializeState 
CDummyHTTPServerResource::Deserialize( const CORE::CDynamicBuffer& inputBuffer ,
                                         const CString& representation           )
{GUCEF_TRACE;

    // For the dummy resource everything always fake succeeds
    return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
