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

#ifndef GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#define GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_COMCORE_CIPUBSUBMSG_H
#include "gucefCOMCORE_CIPubSubMsg.h"
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

/**
 *  Options for the basic serializer for generalized Pub Sub messages
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubMsgBinarySerializerOptions : public CORE::CIConfigurable
{
    public:

    CPubSubMsgBinarySerializerOptions( void );

    virtual ~CPubSubMsgBinarySerializerOptions();

    virtual bool SaveConfig( CORE::CDataNode& config ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& config ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    UInt32 ToOptionsBitMask( void ) const;

    void FromOptionsBitMask( UInt32 bitMask );

    bool msgIdIncluded;
    bool msgDateTimeIncluded;
    bool msgDateTimeAsMsSinceUnixEpochInUtc;
    bool msgPrimaryPayloadIncluded;
    bool msgKeyValuePairsIncluded;
};

/*-------------------------------------------------------------------------*/

/**
 *  Basic serializer for generalized Pub Sub messages
 *
 *  Please note that the options passed should match between Serialize() and Deserialize()
 *  or the binary format will not be compatible 
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubMsgBinarySerializer
{
    public:

    public:
    
    static bool Serialize( const CPubSubMsgBinarySerializerOptions& options ,
                           const CIPubSubMsg& msg                           , 
                           UInt32 currentTargetOffset                       , 
                           CORE::CDynamicBuffer& target                     , 
                           UInt32& bytesWritten                             );

    static bool Deserialize( const CPubSubMsgBinarySerializerOptions& options ,
                             CIPubSubMsg& msg                                 , 
                             UInt32 currentSourceOffset                       , 
                             const CORE::CDynamicBuffer& source               ,
                             UInt32& bytesRead                                ); 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H ? */
