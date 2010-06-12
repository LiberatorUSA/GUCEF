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

#ifndef GUCEF_CORE_CCONFIGSTORECODEC_H
#define GUCEF_CORE_CCONFIGSTORECODEC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"      /* abstract base class for cloneable objects */
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

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
 *      Abstract base class for configuration storage codecs
 *      Decending classes should implement the actual storage and
 *      retrieval methods.
 */
class GUCEF_CORE_PUBLIC_CPP CConfigStoreCodec : public CICloneable
{
        public:

        /**
         *      Constructs a default object.
         *      Does nothing special atm.
         */
        CConfigStoreCodec( void );

        /**
         *      Constructs a copy object.
         *      Does nothing special atm.
         *
         *      @param src the object that is to be used as an template
         */
        CConfigStoreCodec( const CConfigStoreCodec& src );

        /**
         *      Turns this object into a copy of the given object.
         *      Does nothing special atm.
         *
         *      @param src the object that is to be used as an template
         *      @return the modified object
         */
        CConfigStoreCodec& operator=( const CConfigStoreCodec& src );

        /**
         *      Does nothing special atm.
         */
        virtual ~CConfigStoreCodec();

        virtual void SetConfigItem( const CString& configname    ,
                                    const CString& setname       ,
                                    const CString& itemname      ,
                                    const CString& attributename ,
                                    const CString& value         ) = 0;

        virtual void GetConfigItem( const CString& configname    ,
                                    const CString& setname       ,
                                    const CString& itemname      ,
                                    const CString& attributename ,
                                    const CString& value         ) = 0;

        virtual void SetConfigItemSet( const CString& configname        ,
                                       const CString& setname           ,
                                       const CStringList& itemname      ,
                                       const CStringList& attributename ,
                                       const CStringList& value         ) = 0;

        virtual void GetConfigItemSet( const CString& configname        ,
                                       const CString& setname           ,
                                       const CStringList& itemname      ,
                                       const CStringList& attributename ,
                                       const CStringList& value         ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CCONFIGSTORECODEC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-02-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/
