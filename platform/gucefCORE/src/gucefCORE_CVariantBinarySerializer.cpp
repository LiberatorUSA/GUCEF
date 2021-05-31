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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#include "gucefCORE_CVariantBinarySerializer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CVariantBinarySerializer::Serialize( const CVariant& var, UInt32 currentTargetOffset, CDynamicBuffer& target, UInt32& bytesWritten )
{GUCEF_TRACE;
    
    try
    {
        UInt32 lastBytesWritten = 0;
    
        UInt8 typeId = var.GetTypeId();
        lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(typeId), &typeId );
        currentTargetOffset += lastBytesWritten;
        bytesWritten += lastBytesWritten;

        // early exit for unknown type, think of this as a placeholder for a nil value
        if ( GUCEF_DATATYPE_UNKNOWN == typeId )
            return true;

        // if a variable length is possible we need to prefix the size
        UInt32 payloadSize = var.ByteSize();
        if ( var.UsesDynamicMemory() )
        {        
            lastBytesWritten = target.CopyFrom( currentTargetOffset, sizeof(payloadSize), &payloadSize );
            currentTargetOffset += lastBytesWritten;
            bytesWritten += lastBytesWritten;
        }

        lastBytesWritten = target.CopyFrom( currentTargetOffset, payloadSize, var.AsVoidPtr() );
        currentTargetOffset += lastBytesWritten;
        bytesWritten += lastBytesWritten;

        return true;
    }   
    catch ( const std::exception& e )    
    {
        GUCEF_EXCEPTION_LOG( LOGLEVEL_NORMAL, CString( "VariantBinarySerializer:Serialize: caught exception: " ) + e.what()  );
        return false;
    } 
}

/*-------------------------------------------------------------------------*/

bool 
CVariantBinarySerializer::Deserialize( CVariant& var, UInt32 currentSourceOffset, const CDynamicBuffer& source, bool linkWherePossible, UInt32& bytesRead )
{GUCEF_TRACE;

    try
    {
        var.Clear();

        UInt8 typeId = source.AsConstType< UInt8 >( currentSourceOffset );
        currentSourceOffset += sizeof( typeId );
        bytesRead += sizeof( typeId );

        // early exit for unknown type, think of this as a placeholder for a nil value
        if ( GUCEF_DATATYPE_UNKNOWN == typeId )
            return true;

        if ( CVariant::UsesDynamicMemory( typeId ) )
        {
            UInt32 payloadSize = source.AsConstType< UInt32 >( currentSourceOffset );
            currentSourceOffset += sizeof( payloadSize );
            bytesRead += sizeof( payloadSize );

            bool result = var.Set( source.GetConstBufferPtr( currentSourceOffset ), payloadSize, typeId, linkWherePossible );
            bytesRead += var.ByteSize();
            return result;
        }
        else
        {
            bool result = var.Set( source.GetConstBufferPtr( currentSourceOffset ), source.GetDataSize() - currentSourceOffset, typeId, linkWherePossible );
            bytesRead += var.ByteSize();
            return result;            
        }
    }   
    catch ( const std::exception& e )    
    {
        GUCEF_EXCEPTION_LOG( LOGLEVEL_NORMAL, CString( "VariantBinarySerializer:Deserialize: caught exception: " ) + e.what()  );
        return false;
    } 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
