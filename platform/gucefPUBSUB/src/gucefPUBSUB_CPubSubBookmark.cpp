/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefPUBSUB_CPubSubBookmark.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubBookmark::CPubSubBookmark( void )
    : m_bmType( BOOKMARK_TYPE_NOT_INITIALIZED )
    , m_bmData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubBookmark::CPubSubBookmark( TBookmarkType bmType )
    : m_bmType( bmType )
    , m_bmData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubBookmark::CPubSubBookmark( TBookmarkType bmType, const CORE::CVariant& bmData )
    : m_bmType( bmType )
    , m_bmData( bmData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubBookmark::~CPubSubBookmark()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPubSubBookmark::SetBookmarkType( TBookmarkType bmType )
{GUCEF_TRACE;

    m_bmType = bmType;
}
/*-------------------------------------------------------------------------*/

CPubSubBookmark::TBookmarkType
CPubSubBookmark::GetBookmarkType( void ) const
{GUCEF_TRACE;

    return m_bmType;
}

/*-------------------------------------------------------------------------*/

void
CPubSubBookmark::SetBookmarkData( const CORE::CVariant& bmData )
{GUCEF_TRACE;

    m_bmData = bmData;
}
/*-------------------------------------------------------------------------*/

const CORE::CVariant&
CPubSubBookmark::GetBookmarkData( void ) const
{GUCEF_TRACE;

    return m_bmData;
}

/*-------------------------------------------------------------------------*/

CORE::CVariant&
CPubSubBookmark::GetBookmarkData( void )
{GUCEF_TRACE;

    return m_bmData;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
