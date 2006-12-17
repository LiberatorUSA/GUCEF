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

#ifndef GUCEF_IMAGE_CIMGCODECPLUGIN_H
#define GUCEF_IMAGE_CIMGCODECPLUGIN_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"             /* CORE Library of the GUCEFramework */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEF_IMAGE_MACROS_H
#include "gucefIMAGE_macros.h"     /* IMAGE Library macros and build config */
#define GUCEF_IMAGE_MACROS_H
#endif /* GUCEF_IMAGE_MACROS_H ? */

#ifndef GUCEF_IMAGE_IMAGEDATA_H
#include "imagedata.h"             /* image data storage */
#define GUCEF_IMAGE_IMAGEDATA_H
#endif /* GUCEF_IMAGE_IMAGEDATA_H ? */

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

class EXPORT_CPP CIMGCodecPlugin : public CORE::CIPlugin
{
        public:       

        typedef CORE::CTSharedPtr< CIMGCodec > CIMGCodecPtr;
        typedef std::vector< CIMGCodecPtr > CIMGCodecPtrList;

        CIMGCodecPlugin( void );

        ~CIMGCodecPlugin();
        
        bool LoadPlugin( const CORE::CString& filename );
        
        bool UnloadPlugin( void );
        
        bool IsPluginLoaded( void ) const;
        
        virtual const CORE::CString& GetModulePath( void ) const;

        virtual CORE::CString GetName( void ) const;

        virtual CORE::CString GetCopyright( void ) const;

        virtual const CORE::TVersion GetVersion( void ) const;

        UInt32 GetLoadCount( void ) const;

        bool GetCodecList( CIMGCodecPtrList& codecList );

        private:
        friend class CIMGCodecPluginItem;
        
        typedef CIMGCodec::TDynamicBufferList TDynamicBufferList;
        
        bool Encode( const void* sourceData         ,
                     const UInt32 sourceBuffersSize ,
                     TDynamicBufferList& dest       ,
                     UInt32& destBuffersUsed        ,
                     const CORE::CString& typeName  );

        bool Decode( const void* sourceData         ,
                     const UInt32 sourceBuffersSize ,
                     TDynamicBufferList& dest       ,
                     UInt32& destBuffersUsed        ,
                     const CORE::CString& typeName  );        
        
        private:
        
        CIMGCodecPlugin( const CIMGCodecPlugin& src );            /**< should never be used */
        CIMGCodecPlugin& operator=( const CIMGCodecPlugin& src ); /**< should never be used */

        private:
        
        void* m_fptable[ 10 ];  /**< function pointer table */
        void* m_sohandle;       /**< access to the so module */
        mutable void* m_pluginData;
        CIMGCodecPtrList m_codecList;
        CORE::CString m_modulePath;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CIMGCODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 15-12-2006 :
        - Dinand: rewrote this class. separated image loading/saving from the  
                  plugin related functionality
- 26-11-2004 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
