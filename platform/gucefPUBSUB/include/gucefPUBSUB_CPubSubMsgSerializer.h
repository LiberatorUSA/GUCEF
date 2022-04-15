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

#ifndef GUCEF_COMCORE_CPUBSUBMSGSERIALIZER_H
#define GUCEF_COMCORE_CPUBSUBMSGSERIALIZER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CMESSAGESERIALIZER_H
#include "gucefCOMCORE_CMessageSerializer.h"
#define GUCEF_COMCORE_CMESSAGESERIALIZER_H
#endif /* GUCEF_COMCORE_CMESSAGESERIALIZER_H ? */

#ifndef GUCEF_COMCORE_CIPUBSUBMSG_H
#include "gucefPUBSUB_CIPubSubMsg.h"
#define GUCEF_COMCORE_CIPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CIPUBSUBMSG_H ? */

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

class CPubSubClientFeatures;

/**
 *  Options for the basic serializer for generalized Pub Sub messages
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubMsgSerializerOptions : public CMessageSerializerOptions
{
    public:

    static const CString ClassTypeName;

    CPubSubMsgSerializerOptions( void );

    CPubSubMsgSerializerOptions( const CORE::CDataNodeSerializableSettings& basicNodeOptions );

    virtual ~CPubSubMsgSerializerOptions();

    virtual bool SaveConfig( CORE::CDataNode& config ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& config ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    UInt32 ToOptionsBitMask( void ) const;

    void FromOptionsBitMask( UInt32 bitMask );

    void FromPubSubClientFeatures( const CPubSubClientFeatures& features );

    bool receiveActionIdIncluded;
    bool originTopicObjPointerIncluded;
    bool originTopicNameIncluded;
    bool originClientTypeNameIncluded;
    bool originClientObjPointerIncluded;
};

/*-------------------------------------------------------------------------*/

/**
 *  Basic serializer for generalized Pub Sub messages
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubMsgSerializer : public CMessageSerializer
{
    public:

    static bool Serialize( const CPubSubMsgSerializerOptions& options ,
                           const CIPubSubMsg& msg                     ,
                           CORE::CDataNode& output                    );

    static bool Serialize( const CORE::CDataNodeSerializableSettings& basicNodeOptions ,
                           const CIPubSubMsg& msg                                      ,
                           CORE::CDataNode& output                                     );

    static bool Serialize( const CPubSubMsgSerializerOptions& options ,
                           const CIPubSubMsg& msg                     ,
                           CORE::CDynamicBuffer& outputBuffer         ,
                           const CORE::CString& dstoreCodecToUse      );

    static bool Deserialize( const CPubSubMsgSerializerOptions& options ,
                             CIPubSubMsg& msg                           ,
                             const CORE::CDataNode& input               );

    static bool Deserialize( const CPubSubMsgSerializerOptions& options ,
                             CIPubSubMsg& msg                           ,
                             const CORE::CDynamicBuffer& inputBuffer    ,
                             const CORE::CString& dstoreCodecToUse      ); 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPUBSUBMSGSERIALIZER_H ? */
