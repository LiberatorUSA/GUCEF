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

#include "gucefCOM_CDefaultHTTPServerResource.h"

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

CDefaultHTTPServerResource::CDefaultHTTPServerResource( void )
    : CIHTTPServerResource()
    , m_cacheabilityProperty()
    , m_lastModifiedProperty()
    , m_resourceVersionProperty()
    , m_encodingProperty()
    , m_urlSegment()
    , m_deserializationReps()
    , m_serializationReps()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerResource::CDefaultHTTPServerResource( const CDefaultHTTPServerResource& src )
    : CIHTTPServerResource( src )
    , m_cacheabilityProperty( src.m_cacheabilityProperty )
    , m_lastModifiedProperty( src.m_lastModifiedProperty )
    , m_resourceVersionProperty( src.m_resourceVersionProperty )
    , m_encodingProperty( src.m_encodingProperty )
    , m_urlSegment( src.m_urlSegment )
    , m_deserializationReps( src.m_deserializationReps )
    , m_serializationReps( src.m_serializationReps )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerResource::~CDefaultHTTPServerResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerResource&
CDefaultHTTPServerResource::operator=( const CDefaultHTTPServerResource& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_cacheabilityProperty = src.m_cacheabilityProperty;
        m_lastModifiedProperty = src.m_lastModifiedProperty;
        m_resourceVersionProperty = src.m_resourceVersionProperty;
        m_encodingProperty = src.m_encodingProperty;
        m_urlSegment = src.m_urlSegment;
        m_deserializationReps = src.m_deserializationReps;
        m_serializationReps = src.m_serializationReps;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDefaultHTTPServerResource::SetURL( const CString& url )
{GUCEF_TRACE;

    m_urlSegment = url;
}

/*-------------------------------------------------------------------------*/

const CString& 
CDefaultHTTPServerResource::GetURL( void ) const
{GUCEF_TRACE;

    return m_urlSegment;
}

/*-------------------------------------------------------------------------*/

const CString& 
CDefaultHTTPServerResource::GetEncoding( void ) const
{GUCEF_TRACE;

    return m_encodingProperty;
}

/*-------------------------------------------------------------------------*/
                    
const CDefaultHTTPServerResource::TStringVector& 
CDefaultHTTPServerResource::GetSupportedSerializationRepresentations()
{GUCEF_TRACE;

    return m_serializationReps;
}

/*-------------------------------------------------------------------------*/

CString 
CDefaultHTTPServerResource::GetBestMatchedSerializationRepresentation( const CDefaultHTTPServerResource::TStringVector& representations )
{GUCEF_TRACE;

    // The default assumption is that representations have been stored in order of most desireable
    // As such we look for a match between our list in order against the options given
    TStringVector::iterator i = m_serializationReps.begin();
    while ( i != m_serializationReps.end() )
    {
        // Does the client support this representation?
        TStringVector::const_iterator n = representations.begin();
        while ( n != representations.end() )
        { 
            if ( (*i) == (*n) )
            {
                // We found the most desireable representation based on the server-side
                // priority ordered list
                return (*i);
            }
            ++n;
        }
        ++i;
    }

    // No supported agreeable representation found that both sides can agree on 
    // and support
    return CString();
}

/*-------------------------------------------------------------------------*/

const CDefaultHTTPServerResource::TStringVector& 
CDefaultHTTPServerResource::GetSupportedDeserializationRepresentations()
{GUCEF_TRACE;

    return m_deserializationReps;
}

/*-------------------------------------------------------------------------*/

CString 
CDefaultHTTPServerResource::GetBestSupportedDeserializationRepresentation( const TStringVector& representations )
{GUCEF_TRACE;

    // The default assumption is that representations have been stored in order of most desireable
    // As such we look for a match between our list in order against the options given
    TStringVector::iterator i = m_deserializationReps.begin();
    while ( i != m_deserializationReps.end() )
    {
        // Does the client support this representation?
        TStringVector::const_iterator n = representations.begin();
        while ( n != representations.end() )
        { 
            if ( (*i) == (*n) )
            {
                // We found the most desireable representation based on the server-side
                // priority ordered list
                return (*i);
            }
            ++n;
        }
        ++i;
    }

    // No supported agreeable representation found that both sides can agree on 
    // and support
    return CString();
}

/*-------------------------------------------------------------------------*/

CDefaultHTTPServerResource::TCreateState 
CDefaultHTTPServerResource::CreateResource( const CString& transactionID                  ,
                                            const CORE::CDynamicBuffer& inputBuffer       ,
                                            const CString& representation                 ,
                                            THTTPServerResourcePtr& resourceOutput        ,
                                            TStringVector& supportedRepresentationsOutput )
{GUCEF_TRACE;

    // By default we don't allow or implement more complex interaction patterns
    // such as dynamic resources or collections
    return TCreateState::CREATESTATE_FAILED;
}

/*-------------------------------------------------------------------------*/
    
bool 
CDefaultHTTPServerResource::DeleteResource()
{GUCEF_TRACE;

    // By default we don't allow or implement more complex interaction patterns
    // such as dynamic resources or collections
    return false;
}

/*-------------------------------------------------------------------------*/

const CString& 
CDefaultHTTPServerResource::GetResourceVersion( void )
{GUCEF_TRACE;

    return m_resourceVersionProperty;
}

/*-------------------------------------------------------------------------*/
    
const CString& 
CDefaultHTTPServerResource::GetLastModifiedTime( void )
{GUCEF_TRACE;

    return m_lastModifiedProperty;
}

/*-------------------------------------------------------------------------*/

const CString& 
CDefaultHTTPServerResource::GetCacheability( void )
{GUCEF_TRACE;

    return m_cacheabilityProperty;
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHTTPServerResource::Serialize( CORE::CDynamicBuffer& outputBuffer ,
                                       const CString& representation      )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

CDefaultHTTPServerResource::TDeserializeState 
CDefaultHTTPServerResource::Deserialize( const CORE::CDynamicBuffer& inputBuffer ,
                                         const CString& representation           )
{GUCEF_TRACE;

    return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
