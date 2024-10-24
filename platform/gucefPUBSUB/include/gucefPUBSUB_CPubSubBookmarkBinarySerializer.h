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

#ifndef GUCEF_PUBSUB_CPUBSUBBOOKMARKBINARYSERIALIZER_H
#define GUCEF_PUBSUB_CPUBSUBBOOKMARKBINARYSERIALIZER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#include "gucefPUBSUB_CPubSubBookmark.h"
#define GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#endif /* GUCEF_PUBSUB_CPUBSUBBOOKMARK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class providing a binary format serializer/deserializer for pubsub bookmark objects
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubBookmarkBinarySerializer
{
    public:

    static const CORE::CString  MagicText;
    static const CORE::UInt8    CurrentFormatVersion;

    /**
     *  Serializes a pubsub bookmark object to the provided buffer
     */
    static bool Serialize( const CPubSubBookmark& bookmark ,
                           UInt32 currentTargetOffset      , 
                           CORE::CDynamicBuffer& target    , 
                           UInt32& bytesWritten            );

    /**
     *  Serializes a pubsub bookmark object to the provided buffer
     */
    static bool Serialize( const CPubSubBookmark& bookmark ,
                           CORE::CDynamicBuffer& target    );

    /**
     *  Deserializes a pubsub bookmark object from the provided buffer
     */
    static bool Deserialize( CPubSubBookmark& bookmark          ,
                             UInt32 currentSourceOffset         ,
                             bool linkWherePossible             ,
                             const CORE::CDynamicBuffer& source ,
                             bool& isCorrupted                  ,
                             UInt32& bytesRead                  );

    /**
     *  Deserializes a pubsub bookmark object from the provided buffer
     */
    static bool Deserialize( CPubSubBookmark& bookmark          ,
                             bool linkWherePossible             ,
                             const CORE::CDynamicBuffer& source );

    CPubSubBookmarkBinarySerializer( void );
    ~CPubSubBookmarkBinarySerializer();
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBBOOKMARKBINARYSERIALIZER_H ? */

