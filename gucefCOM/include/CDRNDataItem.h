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

#ifndef CDRNDATAITEM_H
#define CDRNDATAITEM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDVSTRING_H
#include "CDVString.h"
#define CDVSTRING_H
#endif /* CDVSTRING_H ? */

#ifndef CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define CDYNAMICBUFFER_H
#endif /* CDYNAMICBUFFER_H ? */

#ifndef CNOTIFIER_H
#include "CNotifier.h"
#define CNOTIFIER_H
#endif /* CNOTIFIER_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */ 

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

class CDRNClient;
class CDRNController;

/*-------------------------------------------------------------------------*/

class GUCEFCOM_EXPORT_CPP CDRNDataItem : public CORE::CNotifier
{
        public:                       
        
        bool operator==( const CDRNDataItem& other ) const;
        
        bool operator!=( const CDRNDataItem& other ) const;
        
        UInt32 GetSize( void ) const;
        
        UInt32 GetItemID( void ) const;                
        
        Float32 AsFloat32( void ) const;
        
        UInt32 AsUInt32( void ) const;
        
        Int32 AsInt32( void ) const;
        
        UInt16 AsUInt16( void ) const;
        
        Int16 AsInt16( void ) const;
        
        UInt8 AsUInt8( void ) const;
        
        Int8 AsInt8( void ) const;
        
        bool AsBool( void ) const;
        
        CORE::CString AsString( void ) const;
        
        CORE::CDynamicBuffer AsBuffer( void ) const;
        
        void CopyTo( CORE::CDynamicBuffer& dest );
        
        void Get( void* data        , 
                  UInt32 buffersize ) const;
                  
        virtual bool IsInitialized( void ) const = 0;                          

        protected:
        friend class CDRNClient;
        friend class CDRNController;
        
        CDRNDataItem( UInt32 itemid );
        
        CDRNDataItem( const CDRNDataItem& src );
        
        virtual ~CDRNDataItem();
        
        CDRNDataItem& operator=( const CDRNDataItem& src );
        
        protected:
        GUCEF::CORE::CDynamicBuffer m_data;
        
        private:
        
        CDRNDataItem( void );
        UInt32 m_itemid;        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNDATAITEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
