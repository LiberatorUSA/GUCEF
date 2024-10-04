/*
 *  gucefKAITAI: Platform module supporting the Kaitai binary description format
 *
 *  Copyright (C) 1998 - 2023.  Dinand Vanvelzen
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

#ifndef GUCEF_KAITAI_CDATASTOREKAITAICODEC_H
#define GUCEF_KAITAI_CDATASTOREKAITAICODEC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDSTORECODEC_H
#include "CDStoreCodec.h"
#define GUCEF_CORE_CDSTORECODEC_H
#endif /* GUCEF_CORE_CDSTORECODEC_H ? */

#ifndef GUCEF_KAITAI_MACROS_H
#include "gucefKAITAI_macros.h"
#define GUCEF_KAITAI_MACROS_H
#endif /* GUCEF_KAITAI_MACROS_H ? */

#ifndef GUCEF_KAITAI_CKAITAISCHEMA_H
#include "gucefKAITAI_CKaitaiSchema.h"
#define GUCEF_KAITAI_CKAITAISCHEMA_H
#endif /* GUCEF_KAITAI_CKAITAISCHEMA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace KAITAI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Data store codec providing the ability to use the KAITAI binary parsing capabilities
 */
class GUCEF_KAITAI_PUBLIC_CPP CDataStoreKaitaiCodec : public CORE::CDStoreCodec
{
    public:

    CDataStoreKaitaiCodec( void );

    virtual ~CDataStoreKaitaiCodec() GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Whether or not the codec is currently valid
     *
     *      @return validity of the codec
     */
    virtual bool IsValid( void ) const;

    /**
     *  Attempts to store the given tree in the file
     *  given according to the method of the codec meta data
     *
     *  @param tree the data tree you wish to store
     *  @param file access to the resource to which the data will be written
     *  @return whether storing the tree was successful
     */
    virtual bool StoreDataTree( const CORE::CDataNode* tree   ,
                                CORE::CIOAccess* file         ) GUCEF_VIRTUAL_OVERRIDE;

     /**
      * Attempts to serialize data from the given data tree to the given output
      * utilizes the Kaitai schema given
      */
    bool Serialize( TKaitaiSchemaPtr schema       ,
                    const CORE::CDataNode* tree   ,
                    CORE::CIOAccess* file         );

    /**
     *      Attempts to load data from the given file to the
     *      root node given. The root data will be replaced
     *      and any children the node may already have will be deleted.
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @param file media independent access to the data source from which the data tree information should be loaded
     *      @return whether building the tree from the given file was successful.
     */
    virtual bool BuildDataTree( CORE::CDataNode* treeroot ,
                                CORE::CIOAccess* file     ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Returns the name of the codec
     *      The author of the codec can give an codec a name that the user can
     *      obtain using this member function.
     *
     *      @return name of the codec
     */
    virtual CORE::CString GetName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Get the name of the type that this codec implements
     *      support for
     *
     *      @return the name of the resource type supported by this codec
     */
    virtual CORE::CString GetTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Returns the copyright of the codec
     *      The author of the codec can give an codec a copyright notice that the user can
     *      obtain using this member function.
     *
     *      @return copyright notice of the codec
     */
    virtual CORE::CString GetCopyright( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Returns the codec version
     *
     *      @return the version of the codec
     */
    virtual CORE::TVersion GetVersion( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    private:
    
    CDataStoreKaitaiCodec( const CDataStoreKaitaiCodec& src );             /**< no sense in an copied codec */
    CDataStoreKaitaiCodec& operator=( const CDataStoreKaitaiCodec& src );  /**< no sense in an copied codec */
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAITAI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_KAITAI_CDATASTOREKAITAICODEC_H ? */
