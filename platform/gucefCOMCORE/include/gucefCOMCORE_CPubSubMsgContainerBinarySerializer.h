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

    typedef std::vector< UInt32 >   TMsgOffsetIndex;

    static const CORE::CString MagicText;

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

    static bool Serialize( const CPubSubMsgBinarySerializerOptions& options     ,
                           const CPubSubClientTopic::TPubSubMsgsRefVector& msgs ,
                           UInt32 currentTargetOffset                           , 
                           CORE::CDynamicBuffer& target                         , 
                           UInt32& bytesWritten                                 );

    static bool DeserializeMsgAtIndex( CIPubSubMsg& msg                   ,
                                       bool linkWherePossible             ,
                                       const CORE::CDynamicBuffer& source ,
                                       UInt32 msgIndex                    ,
                                       bool fromStart                     );
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
