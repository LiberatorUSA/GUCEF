/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#ifndef GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#define GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#include "gucefCOMCORE_CPubSubClientTopic.h"    
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefCOMCORE_CPubSubMsgBinarySerializer.h"
#define GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef GUCEF_COMCORE_CBASICPUBSUBMSG_H
#include "gucefCOMCORE_CBasicPubSubMsg.h"
#define GUCEF_COMCORE_CBASICPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CBASICPUBSUBMSG_H ? */

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
 *  Basic serializer for a container of generalized Pub Sub messages
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubMsgContainerBinarySerializer
{
    public:
    
    typedef std::vector< UInt32 >                   TMsgOffsetIndex;
    typedef CBasicPubSubMsg::TBasicPubSubMsgVector  TBasicPubSubMsgVector;

    static const CORE::CString  MagicText;
    static const CORE::UInt8    CurrentFormatVersion;

    static bool SerializeHeader( const CPubSubMsgBinarySerializerOptions& options ,
                                 UInt32 currentTargetOffset                       , 
                                 CORE::CDynamicBuffer& target                     , 
                                 UInt32& bytesWritten                             );

    static bool DeserializeHeader( CPubSubMsgBinarySerializerOptions& options ,
                                   UInt32 currentSourceOffset                 , 
                                   const CORE::CDynamicBuffer& source         , 
                                   UInt32& bytesRead                          );

    static bool DeserializeHeader( CPubSubMsgBinarySerializerOptions& options ,
                                   const CORE::CDynamicBuffer& source         , 
                                   UInt32& bytesRead                          );

    static bool SerializeFooter( const TMsgOffsetIndex& index ,
                                 UInt32 currentTargetOffset   , 
                                 CORE::CDynamicBuffer& target , 
                                 UInt32& bytesWritten         );

    static bool DeserializeFooter( TMsgOffsetIndex& index             ,
                                   UInt32 currentSourceOffset         , 
                                   const CORE::CDynamicBuffer& source , 
                                   UInt32& bytesRead                  );

    static bool DeserializeFooter( TMsgOffsetIndex& index             ,
                                   const CORE::CDynamicBuffer& source , 
                                   UInt32& bytesRead                  );
    
    /**
     *  If you have a partially written container with a header but no valid 
     *  footer this function can be used to rebuild an index which you could optionally
     *  use to append a new footer at the end of the file with the discovered messages
     *  by passing the forwardOrderedIndex to SerializeFooter()
     */
    static bool IndexRebuildScan( TMsgOffsetIndex& forwardOrderedIndex ,
                                  const CORE::CDynamicBuffer& source   , 
                                  UInt32& bytesRead                    );

    static bool Serialize( const CPubSubMsgBinarySerializerOptions& options     ,
                           const CPubSubClientTopic::TPubSubMsgsRefVector& msgs ,
                           UInt32 currentTargetOffset                           , 
                           CORE::CDynamicBuffer& target                         , 
                           UInt32& bytesWritten                                 );


    /**
     *  Deserializes all the messages from the source buffer using the already 
     *  deserialized index. It is assumed that you already have a pre-allocated 
     *  set of IPubSubMsg based message objects passed in msgs based on the 
     *  previously deserialized footer of which you also need to pass the index
     */
    static bool Deserialize( CPubSubClientTopic::TPubSubMsgsRefVector& msgs ,
                             bool linkWherePossible                         ,
                             const TMsgOffsetIndex& index                   ,
                             const CORE::CDynamicBuffer& source             ,
                             bool& isCorrupted                              );

    static bool Deserialize( TBasicPubSubMsgVector& msgs        ,
                             bool linkWherePossible             ,
                             const TMsgOffsetIndex& index       ,
                             const CORE::CDynamicBuffer& source ,
                             bool& isCorrupted                  );


    /**
     *  If you only need to read a specific message such as the last message this is a
     *  useful helper function to do so. Please do not use this function to iterate all
     *  messages as it would be rather inefficient, code your own iteration for that
     *
     *  Note that if isCorrupted is true you may wish to think about using IndexRebuildScan()
     *  to effect repairs
     */
    static bool DeserializeMsgAtIndex( CIPubSubMsg& msg                   ,
                                       bool linkWherePossible             ,
                                       const CORE::CDynamicBuffer& source ,
                                       UInt32 msgIndex                    ,
                                       bool fromStart                     ,
                                       bool& isCorrupted                  );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */
