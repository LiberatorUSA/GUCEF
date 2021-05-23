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
    , msgPrimaryPayloadIncluded( true )
    , msgKeyValuePairsIncluded( true )
{GUCEF_TRACE;

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
        if ( options.msgIdIncluded )
        {
            // Write the ID using the variable length variant serializer
            if ( !CORE::CVariantBinarySerializer::Serialize( msg.GetMsgId(), currentTargetOffset, target, bytesWritten ) )
                return false;
            currentTargetOffset += bytesWritten;
        }

        if ( options.msgDateTimeIncluded )
        {
            // Write the DateTime of the msg which is ISO 8601 specified fixed length
            // Writing it in string form actually takes up less space vs writing all the binary components
            // We could convert to something like Unix epoch but the few bytes that would save is not worth losing fidelity
            Int32 dtStrSize = msg.GetMsgDateTime().ToIso8601DateTimeString( target, currentTargetOffset, false, true );
            if ( 0 >= dtStrSize )
                return false;
            currentTargetOffset += (UInt32) dtStrSize;
        }

        if ( options.msgPrimaryPayloadIncluded )
        {
            // Now write the primary payload if any exists
            const CORE::CDynamicBuffer& primaryPayload = msg.GetPrimaryPayload().GetData();
            UInt32 primaryPayloadSize = primaryPayload.GetDataSize();
            UInt32 lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(primaryPayloadSize), &primaryPayloadSize );
            currentTargetOffset += lastBytesWritten;
            bytesWritten += lastBytesWritten;
            if ( primaryPayloadSize > 0 )
            {
                lastBytesWritten = target.CopyFrom( primaryPayload, currentTargetOffset );
                if ( lastBytesWritten != primaryPayloadSize )
                    return false;
                currentTargetOffset += lastBytesWritten;
                bytesWritten += lastBytesWritten;
            }
        }

        if ( options.msgKeyValuePairsIncluded )
        {
            const CIPubSubMsg::TKeyValuePayloadLinks& kvPairs = msg.GetKeyValuePairs();
            UInt32 nrOfKvPairs = (UInt32) kvPairs.size();
            UInt32 lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(nrOfKvPairs), &nrOfKvPairs );
            currentTargetOffset += lastBytesWritten;
            bytesWritten += lastBytesWritten;
            CIPubSubMsg::TKeyValuePayloadLinks::const_iterator i = kvPairs.begin();
            while ( i != kvPairs.end() )
            {                
                const CORE::CDynamicBuffer& key = (*i).first.GetData();               
                const CORE::CDynamicBuffer& value = (*i).second.GetData();
                
                UInt32 keySize = key.GetDataSize();
                lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(keySize), &keySize );
                currentTargetOffset += lastBytesWritten;
                bytesWritten += lastBytesWritten;
                if ( keySize > 0 )
                {
                    lastBytesWritten = target.CopyFrom( key, currentTargetOffset );
                    if ( lastBytesWritten != keySize )
                        return false;
                    currentTargetOffset += lastBytesWritten;
                    bytesWritten += lastBytesWritten;
                }

                UInt32 valueSize = value.GetDataSize();
                lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(valueSize), &valueSize );
                currentTargetOffset += lastBytesWritten;
                bytesWritten += lastBytesWritten;
                if ( valueSize > 0 )
                {
                    lastBytesWritten = target.CopyFrom( value, currentTargetOffset );
                    if ( lastBytesWritten != valueSize )
                        return false;
                    currentTargetOffset += lastBytesWritten;
                    bytesWritten += lastBytesWritten;
                }

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
        return false;
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
