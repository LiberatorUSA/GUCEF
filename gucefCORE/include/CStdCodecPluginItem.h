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

#ifndef GUCEF_CORE_CSTDCODECPLUGINITEM_H
#define GUCEF_CORE_CSTDCODECPLUGINITEM_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CODECPLUGINLINK_H
#include "CodecPluginLink.h"
#define GUCEF_CORE_CODECPLUGINLINK_H
#endif /* GUCEF_CORE_CODECPLUGINLINK_H ? */

#ifndef GUCEF_CORE_CICODEC_H
#include "CICodec.h"
#define GUCEF_CORE_CICODEC_H
#endif /* GUCEF_CORE_CICODEC_H ? */

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

class CIOAccess;
class CStdCodecPlugin;

/*-------------------------------------------------------------------------*/

/**
 *  Private class used to redirect from the codec interface to a codec plugin
 *  interface. 
 */
class CStdCodecPluginItem : public CICodec
{
    public:
    
    virtual bool Encode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         CIOAccess& dest                );

    virtual bool Decode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         CIOAccess& dest                );
                         
    virtual CICloneable* Clone( void ) const;
    
    virtual CString GetType( void ) const;
    
    virtual CString GetFamilyName( void ) const;

    private:    
    friend class CStdCodecPlugin;
    
    CStdCodecPluginItem( void* pluginData                   ,
                         const TCodecPluginLink& pluginLink );
    
    CStdCodecPluginItem( const CStdCodecPluginItem& src );
    
    CStdCodecPluginItem& operator=( const CStdCodecPluginItem& src );
    
    virtual ~CStdCodecPluginItem();
    
    private:
    
    TCODECPLUGFPTR_Encode encode;
    TCODECPLUGFPTR_Decode decode;
    CString m_type;
    CString m_familyName;
    void* m_pluginData;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTDCODECPLUGINITEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-07-2005 :
        - Dinand: Added this class

-----------------------------------------------------------------------------*/
