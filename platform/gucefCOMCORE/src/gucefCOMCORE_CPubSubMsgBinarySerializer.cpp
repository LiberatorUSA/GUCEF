/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CVARIANTBINARYSERIALIZER_H
#include "gucefCORE_CVariantBinarySerializer.h"
#define GUCEF_CORE_CVARIANTBINARYSERIALIZER_H
#endif /* GUCEF_CORE_CVARIANTBINARYSERIALIZER_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#include "gucefCOMCORE_CPubSubMsgBinarySerializer.h"

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

CPubSubMsgBinarySerializerOptions::CPubSubMsgBinarySerializerOptions( void )
    : msgIdIncluded( true )
    , msgDateTimeIncluded( true )
    , msgDateTimeAsMsSinceUnixEpochInUtc( true )
    , msgPrimaryPayloadIncluded( true )
    , msgKeyValuePairsIncluded( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubMsgBinarySerializerOptions::~CPubSubMsgBinarySerializerOptions()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgBinarySerializerOptions::SaveConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    config.SetAttribute( "msgDateTimeIncluded", msgDateTimeIncluded );
    config.SetAttribute( "msgDateTimeAsMsSinceUnixEpochInUtc", msgDateTimeAsMsSinceUnixEpochInUtc );
    config.SetAttribute( "msgIdIncluded", msgIdIncluded );
    config.SetAttribute( "msgPrimaryPayloadIncluded", msgPrimaryPayloadIncluded );
    config.SetAttribute( "msgKeyValuePairsIncluded", msgKeyValuePairsIncluded );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgBinarySerializerOptions::LoadConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;

    msgDateTimeIncluded = config.GetAttributeValueOrChildValueByName( "msgDateTimeIncluded" ).AsBool( msgDateTimeIncluded );
    msgDateTimeAsMsSinceUnixEpochInUtc = config.GetAttributeValueOrChildValueByName( "msgDateTimeAsMsSinceUnixEpochInUtc" ).AsBool( msgDateTimeAsMsSinceUnixEpochInUtc );
    msgIdIncluded = config.GetAttributeValueOrChildValueByName( "msgIdIncluded" ).AsBool( msgIdIncluded );
    msgPrimaryPayloadIncluded = config.GetAttributeValueOrChildValueByName( "msgPrimaryPayloadIncluded" ).AsBool( msgPrimaryPayloadIncluded );
    msgKeyValuePairsIncluded = config.GetAttributeValueOrChildValueByName( "msgKeyValuePairsIncluded" ).AsBool( msgKeyValuePairsIncluded );
    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubMsgBinarySerializerOptions::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::CORE::CPubSubMsgBinarySerializerOptions";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

UInt32 
CPubSubMsgBinarySerializerOptions::ToOptionsBitMask( void ) const
{GUCEF_TRACE;

    UInt32 bitMask = 0;
    msgDateTimeIncluded ? GUCEF_SETBITXON( bitMask, 1 ) : GUCEF_SETBITXOFF( bitMask, 1 );
    msgDateTimeAsMsSinceUnixEpochInUtc ? GUCEF_SETBITXON( bitMask, 2 ) : GUCEF_SETBITXOFF( bitMask, 2 );
    msgIdIncluded ? GUCEF_SETBITXON( bitMask, 3 ) : GUCEF_SETBITXOFF( bitMask, 3 );
    msgPrimaryPayloadIncluded ? GUCEF_SETBITXON( bitMask, 4 ) : GUCEF_SETBITXOFF( bitMask, 4 );
    msgKeyValuePairsIncluded ? GUCEF_SETBITXON( bitMask, 5 ) : GUCEF_SETBITXOFF( bitMask, 5 );
    return bitMask;
}

/*-------------------------------------------------------------------------*/

void 
CPubSubMsgBinarySerializerOptions::FromOptionsBitMask( UInt32 bitMask )
{GUCEF_TRACE;

    msgDateTimeIncluded = ( 0 != GUCEF_GETBITX( bitMask, 1 ) );
    msgDateTimeAsMsSinceUnixEpochInUtc = ( 0 != GUCEF_GETBITX( bitMask, 2 ) );
    msgIdIncluded = ( 0 != GUCEF_GETBITX( bitMask, 3 ) );
    msgPrimaryPayloadIncluded = ( 0 != GUCEF_GETBITX( bitMask, 4 ) );
    msgKeyValuePairsIncluded = ( 0 != GUCEF_GETBITX( bitMask, 5 ) );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgBinarySerializer::Serialize( const CPubSubMsgBinarySerializerOptions& options ,
                                       const CIPubSubMsg& msg                           , 
                                       UInt32 currentTargetOffset                       , 
                                       CORE::CDynamicBuffer& target                     , 
                                       UInt32& bytesWritten                             )
{GUCEF_TRACE;
    
    try
    {
        if ( options.msgDateTimeIncluded )
        {
            if ( options.msgDateTimeAsMsSinceUnixEpochInUtc )
            {
                // Write the DateTime of the msg as milliseconds since Unix Epoch in UTC
                // In most applications Unix Epoch is not UTC and leap seconds are not taken into account
                // However since the source if of higher fidelity we can retain accuracy as long as the same method is
                // used to deserialize as well. There is a problem with any DateTime which predates Unix epoch
                // If you have to deal with those use string formatting instead
                UInt64 msSinceUnixEpochInUtc = msg.GetMsgDateTime().ToUnixEpochBasedTicksInMillisecs();
                UInt32 lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(msSinceUnixEpochInUtc), &msSinceUnixEpochInUtc );
                currentTargetOffset += lastBytesWritten;
                bytesWritten += lastBytesWritten;
            }
            else
            {
                // Write the DateTime of the msg which as a ISO 8601 spec string
                // Writing it in string form actually takes up less space vs writing all the binary components at full fidelity
                // Using Unix epoch uses less space but we are limited in the time range since historical dates would be a problem 
                Int32 dtStrSize = msg.GetMsgDateTime().ToIso8601DateTimeString( target, currentTargetOffset, false, true );
                if ( 0 >= dtStrSize )
                    return false;
                currentTargetOffset += (UInt32) dtStrSize;
                bytesWritten += (UInt32) dtStrSize;
            }
        }

        if ( options.msgIdIncluded )
        {
            // Write the ID using the variable length variant serializer
            UInt32 varBinSize = 0;
            if ( !CORE::CVariantBinarySerializer::Serialize( msg.GetMsgId(), currentTargetOffset, target, varBinSize ) )
                return false;
            currentTargetOffset += varBinSize;
            bytesWritten += varBinSize;
        }

        if ( options.msgPrimaryPayloadIncluded )
        {
            // Now write the primary payload if any exists
            const CORE::CVariant& primaryPayload = msg.GetPrimaryPayload();
            UInt32 varBinSize = 0;
            if ( !CORE::CVariantBinarySerializer::Serialize( primaryPayload, currentTargetOffset, target, varBinSize ) )
                return false;
            currentTargetOffset += varBinSize;
            bytesWritten += varBinSize;
        }

        if ( options.msgKeyValuePairsIncluded )
        {
            const CIPubSubMsg::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();
            
            UInt32 nrOfKvPairs = (UInt32) kvPairs.size();
            UInt32 lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(nrOfKvPairs), &nrOfKvPairs );
            currentTargetOffset += lastBytesWritten;
            bytesWritten += lastBytesWritten;

            CIPubSubMsg::TKeyValuePairs::const_iterator i = kvPairs.begin();
            while ( i != kvPairs.end() )
            {                
                const CORE::CVariant& key = (*i).first;               
                const CORE::CVariant& value = (*i).second;

                UInt32 varBinSize = 0;
                if ( !CORE::CVariantBinarySerializer::Serialize( key, currentTargetOffset, target, varBinSize ) )
                    return false;
                currentTargetOffset += varBinSize;
                bytesWritten += varBinSize;
                
                varBinSize = 0;
                if ( !CORE::CVariantBinarySerializer::Serialize( value, currentTargetOffset, target, varBinSize ) )
                    return false;
                currentTargetOffset += varBinSize;
                bytesWritten += varBinSize;

                ++i;
            }
        }

        return true;
    }   
    catch ( const std::exception& e )    
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "PubSubMsgBinarySerializer:Serialize: caught exception: " ) + e.what()  );
        return false;
    } 
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubMsgBinarySerializer::Deserialize( const CPubSubMsgBinarySerializerOptions& options ,
                                         CIPubSubMsg& msg                                 , 
                                         UInt32 currentSourceOffset                       , 
                                         const CORE::CDynamicBuffer& source               , 
                                         UInt32& bytesRead                                )
{GUCEF_TRACE;

    try
    {
        if ( options.msgDateTimeIncluded )
        {
            if ( options.msgDateTimeAsMsSinceUnixEpochInUtc )
            {
                // Read the DateTime of the msg which is milliseconds since Unix epoch in UTC IF
                // this class's Serialize() was used. Commonly used Unix epoch variables do NOT have a  
                // timezone associated nor does the Unix epoch offset in a lot of implementations take
                // leap seconds into account. Hence choose carefully to ensure consistency in interpretation
                UInt64 msSinceUnixEpochInUtc = source.AsConstType< UInt64 >( currentSourceOffset );
                currentSourceOffset += sizeof(msSinceUnixEpochInUtc);
                bytesRead += sizeof(msSinceUnixEpochInUtc);

                msg.GetMsgDateTime().FromUnixEpochBasedTicksInMillisecs( msSinceUnixEpochInUtc );
            }
            else
            {
                // Read the DateTime of the msg which is a ISO 8601 format string
                Int32 dtStrSize = msg.GetMsgDateTime().FromIso8601DateTimeString( source, currentSourceOffset );
                if ( 0 >= dtStrSize )
                    return false;
                currentSourceOffset += (UInt32) dtStrSize;
                bytesRead += (UInt32) dtStrSize;
            }
        }

        if ( options.msgIdIncluded )
        {
            // Write the ID using the variable length variant serializer
            UInt32 varByteSize = 0;
            if ( !CORE::CVariantBinarySerializer::Deserialize( msg.GetMsgId(), currentSourceOffset, source, varByteSize ) )
                return false;
            currentSourceOffset += varByteSize;
            bytesRead += varByteSize;
        }

        if ( options.msgPrimaryPayloadIncluded )
        {
            // Now read the primary payload if any exists
            UInt32 varByteSize = 0;
            if ( !CORE::CVariantBinarySerializer::Deserialize( msg.GetPrimaryPayload(), currentSourceOffset, source, varByteSize ) )
                return false;
            currentSourceOffset += varByteSize;
            bytesRead += varByteSize;
        }

        if ( options.msgKeyValuePairsIncluded )
        {
            // Now read the key-value pairs if any exists
            
            UInt32 nrOfKvPairs = source.AsConstType< UInt32 >( currentSourceOffset );
            currentSourceOffset += sizeof(nrOfKvPairs);
            bytesRead += sizeof(nrOfKvPairs);

            if ( nrOfKvPairs > 0 )
            {
                CIPubSubMsg::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();
                kvPairs.clear();
                kvPairs.resize( nrOfKvPairs );

                CIPubSubMsg::TKeyValuePairs::iterator i = kvPairs.begin();
                while ( i != kvPairs.end() )
                {                
                    CORE::CVariant& key = (*i).first;               
                    CORE::CVariant& value = (*i).second;
                
                    UInt32 varByteSize = 0;
                    if ( !CORE::CVariantBinarySerializer::Deserialize( key, currentSourceOffset, source, varByteSize ) )
                        return false;
                    currentSourceOffset += varByteSize;
                    bytesRead += varByteSize;

                    varByteSize = 0;
                    if ( !CORE::CVariantBinarySerializer::Deserialize( value, currentSourceOffset, source, varByteSize ) )
                        return false;
                    currentSourceOffset += varByteSize;
                    bytesRead += varByteSize;

                    ++i;
                }
            }            
        }

        return true;
    }   
    catch ( const std::exception& e )    
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "PubSubMsgBinarySerializer:Deserialize: caught exception: " ) + e.what()  );
        return false;
    } 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
