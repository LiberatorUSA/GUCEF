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

#ifndef GUCEF_COMCORE_CPUBSUBBOOKMARK_H
#define GUCEF_COMCORE_CPUBSUBBOOKMARK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class providing the a conceptual representation of a backend "bookmark"
 *  A bookmark is a way of keeping track where you are as you consume messages or for tracing
 *  published messages. The actual implementation will vary per backend but the concept is the same
 *  which is simply that, a way of demarkation of where you are.
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubBookmark
{
    public:

    enum EBookmarkType : Int32
    {
        BOOKMARK_TYPE_NOT_INITIALIZED = 0 ,         /**< the backend has not initialized the bookmark information to anything */
        BOOKMARK_TYPE_NOT_AVAILABLE       ,         /**< bookmark information is not available at the moment but the concept is supported */
        BOOKMARK_TYPE_NOT_APPLICABLE      ,         /**< bookmark concept is NA, useless within the context with the given backends */
        
        BOOKMARK_TYPE_MSG_ID              ,
        BOOKMARK_TYPE_MSG_INDEX           ,
        BOOKMARK_TYPE_MSG_DATETIME        ,
        BOOKMARK_TYPE_TOPIC_INDEX  
    };
    typedef enum EBookmarkType TBookmarkType;

    CPubSubBookmark( void );

    CPubSubBookmark( TBookmarkType bmType );

    CPubSubBookmark( TBookmarkType bmType, const CORE::CVariant& bmData );

    virtual ~CPubSubBookmark();

    void SetBookmarkType( TBookmarkType bmType );
    
    TBookmarkType GetBookmarkType( void ) const;

    void SetBookmarkData( const CORE::CVariant& bmData );
    
    const CORE::CVariant& GetBookmarkData( void ) const;

    CORE::CVariant& GetBookmarkData( void );

    private:

    TBookmarkType m_bmType;
    CORE::CVariant m_bmData;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPUBSUBBOOKMARK_H ? */

