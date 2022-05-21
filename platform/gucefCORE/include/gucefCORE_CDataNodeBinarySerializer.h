/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CDATANODEBINARYSERIALIZER_H
#define GUCEF_CORE_CDATANODEBINARYSERIALIZER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Reserved for future use
 */
class GUCEF_CORE_PUBLIC_CPP CDataNodeBinarySerializerOptions : public CIConfigurable
{
    public:

    static const CString ClassTypeName;

    CDataNodeBinarySerializerOptions( void );

    virtual ~CDataNodeBinarySerializerOptions();

    virtual bool SaveConfig( CORE::CDataNode& config ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& config ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

/**
 *  Build-in serializer for a binary representation of a DataNode based DOM
 *  IF you want to use a binary format it would be recommended to use this serializer
 *  reason being that compatibility it garantuees compatiblity between the DataNode class
 *  and any the serialization/deserialization as changes get introduced
 */
class GUCEF_CORE_PUBLIC_CPP CDataNodeBinarySerializer
{
    public:

    static bool Serialize( const CDataNodeBinarySerializerOptions& options ,
                           const CDataNode& node                           , 
                           UInt32 currentTargetOffset                      , 
                           CORE::CDynamicBuffer& target                    , 
                           UInt32& bytesWritten                            );

    /**
     *  Deserializes the binary contents into the DOM referenced by 'node'
     *  Note that the contents of node will be cleared and it will serve as root of the DOM
     */
    static bool Deserialize( const CDataNodeBinarySerializerOptions& options ,
                             bool linkWherePossible                          ,
                             CDataNode& node                                 , 
                             UInt32 currentSourceOffset                      , 
                             const CORE::CDynamicBuffer& source              ,
                             UInt32& bytesRead                               ); 
};

/*-------------------------------------------------------------------------*/

/**
 *  Container serializer that adds a header concept for compatibility
 *  This is the recomended serializer for non-transient or negotiated binary representations
 */
class GUCEF_CORE_PUBLIC_CPP CDataNodeBinaryContainerSerializer
{
    public:

    static const CString  MagicText;
    static const UInt8    CurrentFormatVersion;

    static bool SerializeHeader( const CDataNodeBinarySerializerOptions& options ,
                                 UInt32 currentTargetOffset                      ,
                                 CDynamicBuffer& target                          ,
                                 UInt32& bytesWritten                            );

    static bool Serialize( const CDataNodeBinarySerializerOptions& options ,
                           const CDataNode& node                           , 
                           UInt32 currentTargetOffset                      , 
                           CDynamicBuffer& target                          , 
                           UInt32& bytesWritten                            );

    static bool DeserializeHeader( CDataNodeBinarySerializerOptions& options ,
                                   UInt32 currentSourceOffset                ,
                                   const CORE::CDynamicBuffer& source        ,
                                   UInt32& bytesRead                         );

    /**
     *  Checks the container header for compatibility and type safety
     *  If successfull it deserializes the binary contents into the DOM referenced by 'node'
     *  Note that the contents of node will be cleared and it will serve as root of the DOM
     */
    static bool Deserialize( CDataNodeBinarySerializerOptions& options ,
                             bool linkWherePossible                    ,
                             CDataNode& node                           , 
                             UInt32 currentSourceOffset                , 
                             const CDynamicBuffer& source              ,
                             UInt32& bytesRead                         );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDATANODEBINARYSERIALIZER_H ? */
