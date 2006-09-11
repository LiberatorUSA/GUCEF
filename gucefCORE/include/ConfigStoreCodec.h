/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
class EXPORT_CPP CConfigStoreCodec : public CICloneable
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
