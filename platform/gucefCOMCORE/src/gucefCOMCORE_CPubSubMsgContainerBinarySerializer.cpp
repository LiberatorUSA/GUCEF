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

#include "gucefCOMCORE_CPubSubMsgContainerBinarySerializer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CPubSubMsgContainerBinarySerializer::MagicText = "PUBSUBMSGS";
const CORE::UInt8   CPubSubMsgContainerBinarySerializer::CurrentFormatVersion = 1;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CPubSubMsgContainerBinarySerializer::SerializeHeader( const CPubSubMsgBinarySerializerOptions& options ,
                                                      UInt32 currentTargetOffset                       , 
                                                      CORE::CDynamicBuffer& target                     , 
                                                      UInt32& bytesWritten                             )
{GUCEF_TRACE;

    // magic text bytes to help identify the blob as being a container of pub sub messages
    UInt32 newBytesWritten = target.CopyFrom( MagicText, currentTargetOffset, false );
    bytesWritten += newBytesWritten;
    currentTargetOffset += newBytesWritten; 
    if ( newBytesWritten != MagicText.ByteSize()-1 )
        return false;

    // Reserved for the future version field
    UInt8 formatVersion = CurrentFormatVersion;
    newBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(formatVersion), &formatVersion );
    bytesWritten += newBytesWritten;
    currentTargetOffset += newBytesWritten;
    if ( newBytesWritten != sizeof(formatVersion) )
        return false;

    // Since the serializer settings used have to match to ensure compatibility we will
    // store those in the header so that we know the format of each message
    UInt32 msgSerializerOptions = options.ToOptionsBitMask();
    newBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(msgSerializerOptions), &msgSerializerOptions );
    bytesWritten += newBytesWritten;
    currentTargetOffset += newBytesWritten;
    if ( newBytesWritten != sizeof(msgSerializerOptions) )
        return false;
                 
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgContainerBinarySerializer::DeserializeHeader( CPubSubMsgBinarySerializerOptions& options ,
                                                        UInt32 currentSourceOffset                 , 
                                                        const CORE::CDynamicBuffer& source         , 
                                                        UInt32& bytesRead                          )
{GUCEF_TRACE;

    try
    {
        // magic text bytes to help identify the blob as being a container of pub sub messages
        CORE::CString testStr;    
        UInt32 newBytesRead = source.CopyTo( currentSourceOffset, MagicText.ByteSize()-1, testStr.Reserve( MagicText.ByteSize(), (Int32) MagicText.Length() ) );
        bytesRead += newBytesRead;
        currentSourceOffset += newBytesRead;
        if ( newBytesRead != MagicText.ByteSize()-1 || testStr != MagicText )
        {
            // We dont want to log errors or warnings for this because header reading magic is a typical way to brute force
            // check if the resource is of a certain type
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeHeader: Magic text test failed" );
            return false;
        }

        UInt8 formatVersion = source.AsConstType< UInt8 >( currentSourceOffset );
        bytesRead += sizeof(formatVersion);
        currentSourceOffset += sizeof(formatVersion);
        if ( CurrentFormatVersion != formatVersion )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeHeader: Incompatible format version" );
            return false;
        }

        UInt32 optionsBitMask = source.AsConstType< UInt32 >( currentSourceOffset );
        bytesRead += sizeof(optionsBitMask);
        currentSourceOffset += sizeof(optionsBitMask);
        options.FromOptionsBitMask( optionsBitMask );

        return true;
    }
    catch ( const std::exception& )
    {
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgContainerBinarySerializer::DeserializeHeader( CPubSubMsgBinarySerializerOptions& options ,
                                                        const CORE::CDynamicBuffer& source         , 
                                                        UInt32& bytesRead                          )
{GUCEF_TRACE;

    return DeserializeHeader( options, 0, source, bytesRead );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgContainerBinarySerializer::SerializeFooter( const TMsgOffsetIndex& index ,
                                                      UInt32 currentTargetOffset   , 
                                                      CORE::CDynamicBuffer& target , 
                                                      UInt32& bytesWritten         )
{GUCEF_TRACE;

    UInt32 footerSize = ((UInt32)index.size()*4)+4+(MagicText.ByteSize()-1);
    if ( target.GetUnusedBufferSize() < footerSize )
        target.SetBufferSize( target.GetBufferSize() + footerSize, false );
    
    // Write the msg offset index
    // The purpose of the index is quick access to messages by index even though the
    // messages have variable length
    TMsgOffsetIndex::const_iterator i = index.begin();
    while ( i != index.end() )
    {
        UInt32 msgOffsetInBytes = (*i);
        UInt32 newBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(msgOffsetInBytes), &msgOffsetInBytes );
        bytesWritten += newBytesWritten;
        currentTargetOffset += newBytesWritten;
        if ( newBytesWritten != sizeof(msgOffsetInBytes) )
            return false;

        ++i;
    }

    // Write the msg offset index size
    // The point of this is fast loading of the footer when reading from the end of the container backwards
    UInt32 indexSize = (UInt32) index.size();
    UInt32 newBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(indexSize), &indexSize );
    bytesWritten += newBytesWritten;
    currentTargetOffset += newBytesWritten;
    if ( newBytesWritten != sizeof(indexSize) )
        return false;

    // Now write magic text bytes to help identify the blob as being a complete container of pub sub messages
    // If the magic text was at the start of the container but not at the end you know that the container is potentially
    // only a partial file. At that point the footer/index could potentially be rebuild by scanning the entire container from the start
    newBytesWritten = target.CopyFrom( MagicText, currentTargetOffset, false );
    bytesWritten += newBytesWritten;
    currentTargetOffset += newBytesWritten; 
    if ( newBytesWritten != MagicText.ByteSize()-1 )
        return false;

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubMsgContainerBinarySerializer::DeserializeFooter( TMsgOffsetIndex& index             ,
                                                        UInt32 currentSourceOffset         , 
                                                        const CORE::CDynamicBuffer& source , 
                                                        UInt32& bytesRead                  )
{GUCEF_TRACE;

    try
    {
        // Note that footer reading occurs backwards from the end of the container towards the front

        CORE::CString testStr;    
        UInt32 newBytesRead = source.CopyTo( currentSourceOffset - MagicText.ByteSize()+1, MagicText.ByteSize()-1, testStr.Reserve( MagicText.ByteSize(), (Int32) MagicText.Length() ) );
        if ( newBytesRead != MagicText.ByteSize()-1 || testStr != MagicText )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeMsgAtIndex: Failed to read container footer magic text" );
            return false;
        }
        currentSourceOffset -= MagicText.ByteSize()-1;
        bytesRead += MagicText.ByteSize()-1;

        UInt32 indexSize = 0;
        indexSize = source.AsConstType< UInt32 >( currentSourceOffset - sizeof(indexSize) );
        currentSourceOffset -= sizeof(indexSize);
        bytesRead += sizeof(indexSize);

        // read the msg offset index
        // The purpose of the index is quick access to messages by index even though the
        // messages have variable length
        index.reserve( indexSize );
        for ( UInt32 i=0; i<indexSize; ++i )
        {
            UInt32 msgOffset = 0;
            msgOffset = source.AsConstType< UInt32 >( currentSourceOffset - sizeof(msgOffset) );
            currentSourceOffset -= sizeof(msgOffset);
            bytesRead += sizeof(msgOffset);

            index.push_back( msgOffset );
        }

        return true;
    }
    catch ( const std::exception& )
    {
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubMsgContainerBinarySerializer::DeserializeFooter( TMsgOffsetIndex& index             ,
                                                        const CORE::CDynamicBuffer& source , 
                                                        UInt32& bytesRead                  )
{GUCEF_TRACE;

    return DeserializeFooter( index, source.GetDataSize(), source, bytesRead );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgContainerBinarySerializer::Serialize( const CPubSubMsgBinarySerializerOptions& options     ,
                                                const CPubSubClientTopic::TPubSubMsgsRefVector& msgs ,
                                                UInt32 currentTargetOffset                           , 
                                                CORE::CDynamicBuffer& target                         , 
                                                UInt32& bytesWritten                                 )
{GUCEF_TRACE;

    // Write the header
    UInt32 newBytesWritten = 0;
    bool wroteHeader = SerializeHeader( options, currentTargetOffset, target, newBytesWritten );
    currentTargetOffset += newBytesWritten;
    bytesWritten += newBytesWritten;
    if ( !wroteHeader )
        return false;

    // Write all the messages
    // We do not accept partial success
    TMsgOffsetIndex index( msgs.size(), 0 );
    TMsgOffsetIndex::iterator n = index.begin();
    CPubSubClientTopic::TPubSubMsgsRefVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )
    {
        const CIPubSubMsg* msg = (*i);
        if ( GUCEF_NULL != msg )
        {
            newBytesWritten = 0;
            if ( COMCORE::CPubSubMsgBinarySerializer::Serialize( options, *msg, currentTargetOffset, target, newBytesWritten ) )
            {
                (*n) = currentTargetOffset;
                currentTargetOffset += newBytesWritten;
                bytesWritten += newBytesWritten;
            }
            else
                return false;
        }
        ++i; ++n;
    }

    // Write the footer
    newBytesWritten = 0;
    bool wroteFooter = SerializeFooter( index, currentTargetOffset, target, newBytesWritten );
    currentTargetOffset += newBytesWritten;
    bytesWritten += newBytesWritten;
    if ( !wroteFooter )
        return false;

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubMsgContainerBinarySerializer::DeserializeMsgAtIndex( CIPubSubMsg& msg                   ,
                                                            bool linkWherePossible             ,
                                                            const CORE::CDynamicBuffer& source ,
                                                            UInt32 msgIndex                    ,
                                                            bool fromStart                     )
{GUCEF_TRACE;

    CORE::UInt32 bytesRead = 0;
    COMCORE::CPubSubMsgBinarySerializerOptions options;
    if ( !DeserializeHeader( options, source, bytesRead ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeMsgAtIndex: Failed to read container header" );
        return false;
    }

    // Note that the footer is read in reversed order vs how it was written 
    // thus so will be the entries in the index
    TMsgOffsetIndex index;
    if ( !DeserializeFooter( index, source, bytesRead ) )
    {
        // @TODO: Rebuild index as a fallback
        
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeMsgAtIndex: Failed to read container footer" );
        return false;
    }
    if ( index.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeMsgAtIndex: Footer claims the container is empty" );
        return false;
    }

    if ( msgIndex >= index.size() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeMsgAtIndex: Per footer the index is out of range. Actual Nr of items: " + CORE::ToString( index.size() ) );
        return false;
    }

    UInt32 actualIndex = (UInt32) ( fromStart ? (index.size()-1) - msgIndex : msgIndex );
    CORE::UInt32 offsetOfLastMsg = index[ actualIndex ];
    if ( !COMCORE::CPubSubMsgBinarySerializer::Deserialize( options, msg, offsetOfLastMsg, source, bytesRead ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubMsgContainerBinarySerializer:DeserializeMsgAtIndex: Failed to deserialize msg at index " + CORE::ToString( actualIndex ) + " and offset " + CORE::ToString( offsetOfLastMsg ) );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
