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

#ifndef CDRNLOCALDATAITEM_H
#define CDRNLOCALDATAITEM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDRNDATAITEM_H
#include "CDRNDataItem.h"
#define CDRNDATAITEM_H
#endif /* CDRNDATAITEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CDRNBroadcaster;
class CDRNClient;
class CDRNController;

class CDRNLocalDataItem : public CDRNDataItem
{
        public:                
        
        void Set( const void* data , 
                  UInt32 datasize  );                                                 
        
        CDRNLocalDataItem& operator=( const Float32& floatvalue );       
        
        CDRNLocalDataItem& operator=( const Int8& intvalue );
        
        CDRNLocalDataItem& operator=( const UInt8& intvalue );
        
        CDRNLocalDataItem& operator=( const Int16& intvalue );
        
        CDRNLocalDataItem& operator=( const UInt16& intvalue );
        
        CDRNLocalDataItem& operator=( const Int32& intvalue );
        
        CDRNLocalDataItem& operator=( const UInt32& intvalue );
        
        CDRNLocalDataItem& operator=( const CORE::CString& stringvalue );
        
        CDRNLocalDataItem& operator=( const bool& boolvalue );
        
        virtual bool IsInitialized( void ) const;       
        
        private:
        friend class CDRNClient;
        friend class CDRNController;
 
        CDRNLocalDataItem( CDRNBroadcaster* broadcaster ,
                           UInt32 itemid                );
        
        CDRNLocalDataItem( const CDRNLocalDataItem& src );
        
        virtual ~CDRNLocalDataItem();
        
        CDRNLocalDataItem& operator=( const CDRNLocalDataItem& src );
        
        private:
        
        CDRNBroadcaster* m_broadcaster;
        bool m_initialized;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNLOCALDATAITEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
