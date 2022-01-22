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

#ifndef GUCEF_COMCORE_CMESSAGESERIALIZER_H
#define GUCEF_COMCORE_CMESSAGESERIALIZER_H

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

#ifndef GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#include "gucefCORE_CDataNodeSerializableSettings.h"
#define GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#endif /* GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H ? */

#ifndef GUCEF_COMCORE_CIMESSAGE_H
#include "gucefCOMCORE_CIMessage.h"
#define GUCEF_COMCORE_CIMESSAGE_H
#endif /* GUCEF_COMCORE_CIMESSAGE_H ? */

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
 *  Options for the basic serializer for generalized messages
 */
class GUCEF_COMCORE_EXPORT_CPP CMessageSerializerOptions : public CORE::CDataNodeSerializableSettings
{
    public:
    
    static const CString ClassTypeName;

    CMessageSerializerOptions( void );

    CMessageSerializerOptions( const CORE::CDataNodeSerializableSettings& basicNodeOptions );

    virtual ~CMessageSerializerOptions();

    virtual bool SaveConfig( CORE::CDataNode& config ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& config ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    UInt32 ToOptionsBitMask( void ) const;

    void FromOptionsBitMask( UInt32 bitMask );

    bool msgIdIncluded;
    bool msgIdTypeIncluded;
    bool msgIndexIncluded;
    bool msgIndexTypeIncluded;
    bool msgDateTimeIncluded;
    bool msgDateTimeAsMsSinceUnixEpochInUtc;
    bool msgPrimaryPayloadIncluded;
    bool msgPrimaryPayloadTypeIncluded;
    bool msgKeyValuePairsIncluded;
    bool msgMetaDataKeyValuePairsIncluded;

    bool includeUndefinedValues;
};

/*-------------------------------------------------------------------------*/

/**
 *  Basic serializer for generalized messages
 */
class GUCEF_COMCORE_EXPORT_CPP CMessageSerializer
{
    public:

    static bool SerializeKvPairs( const CIMessage::TKeyValuePairs& kvPairs ,
                                  CORE::CDataNode& output                  );

    static bool Serialize( const CMessageSerializerOptions& options ,
                           const CIMessage& msg                     ,
                           CORE::CDataNode& output                  );

    static bool Serialize( const CMessageSerializerOptions& options ,
                           const CIMessage& msg                     ,
                           CORE::CDynamicBuffer& outputBuffer       ,
                           const CORE::CString& dstoreCodecToUse    );

    static bool Deserialize( const CMessageSerializerOptions& options ,
                             CIMessage& msg                           ,
                             const CORE::CDataNode& input             );

    static bool Deserialize( const CMessageSerializerOptions& options ,
                             CIMessage& msg                           ,
                             const CORE::CDynamicBuffer& inputBuffer  ,
                             const CORE::CString& dstoreCodecToUse    ); 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CMESSAGESERIALIZER_H ? */
