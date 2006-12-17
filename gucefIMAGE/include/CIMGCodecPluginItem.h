/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_IMAGE_CIMGCODECPLUGINITEM_H
#define GUCEF_IMAGE_CIMGCODECPLUGINITEM_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_IMAGE_CIMGCODEC_H
#include "CIMGCodec.h"     
#define GUCEF_IMAGE_CIMGCODEC_H
#endif /* GUCEF_IMAGE_CIMGCODEC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CIMGCodecPlugin;

class CIMGCodecPluginItem : public CIMGCodec
{
    public:

    virtual ~CIMGCodecPluginItem();

    virtual bool Encode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         TDynamicBufferList& dest       ,
                         UInt32& destBuffersUsed        );

    virtual bool Decode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         TDynamicBufferList& dest       ,
                         UInt32& destBuffersUsed        );

    virtual CORE::CICloneable* Clone( void ) const;
    
    virtual CORE::CString GetType( void ) const;
                         
    private:
    friend class CIMGCodecPlugin;
    
    CIMGCodecPluginItem( CIMGCodecPlugin* plugin       ,
                         const CORE::CString& typeName );
                         
    private:

    CIMGCodecPluginItem( void );
    CIMGCodecPluginItem( const CIMGCodecPluginItem& src );
    CIMGCodecPluginItem& operator=( const CIMGCodecPluginItem& src );
    
    private:
    
    CIMGCodecPlugin* m_plugin;
    CORE::CString m_typeName;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CIMGCODECPLUGINITEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 15-12-2006 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/