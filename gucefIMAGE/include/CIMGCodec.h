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

#ifndef CIMGCODEC_H
#define CIMGCODEC_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"             /* CORE Library of the GUCEFramework */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"     /* IMAGE Library macros and build config */
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

#ifndef IMAGEDATA_H
#include "imagedata.h"             /* image data storage */
#define IMAGEDATA_H
#endif /* IMAGEDATA_H ? */

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

class EXPORT_CPP CIMGCodec
{
        public:

        CIMGCodec( const CORE::CString& filename );

        ~CIMGCodec();

        bool GetIsValid( void ) const;
        
        bool CheckIfLoadable( const CORE::CString& filename ,
                              UInt32 *hidx                  );

        bool CheckIfLoadable( CORE::CIOAccess& access ,
                              UInt32 *hidx            );

        bool CheckIfLoadable( Int32 format ,
                              UInt32 *hidx );
                              
        bool CheckIfSaveable( const CORE::CString& filename ,
                              UInt32 *hidx                  );

        bool CheckIfSaveable( const TImageData* imgdata ,
                              UInt32 *hidx              );                              

        bool CheckIfSaveable( Int32 format ,
                              UInt32 *hidx );

        bool Load( UInt32 hidx             ,
                   CORE::CIOAccess& access ,
                   TImageData *imgdata     ,
                   void **plugdata         );

        void Unload( UInt32 hidx         ,
                     TImageData* imgdata ,
                     void* plugdata      );

        bool Save( UInt32 hidx                   ,
                   const CORE::CString& filename ,
                   UInt32 format                 ,
                   UInt32 compression            ,
                   const TImageData *imgdata     );

        CORE::CString GetName( void );

        CORE::CString GetCopyrightEULA( void );

        const CORE::TVersion* GetVersion( void );

        UInt32 GetLoadCount( void ) const;
        
        CORE::CStringList GetLoadableExtList( void ) const;
        
        CORE::CStringList GetSaveableExtList( void ) const;

        private:
        CIMGCodec( const CIMGCodec& src );            /**< should never be used */
        CIMGCodec& operator=( const CIMGCodec& src ); /**< should never be used */

        void* _fptable[ 14 ];  /**< function pointer table */
        UInt32 _loadcount;     /**< number of outstanding image loads */
        void *_sohandle;       /**< access to the so module */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CIMGCODEC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
