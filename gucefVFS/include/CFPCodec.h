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

#ifndef CFPCODEC_H
#define CFPCODEC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CICLONEABLE_H
#include "CICloneable.h"     /* abstract base class for cloneable objects */
#define CICLONEABLE_H
#endif /* CICLONEABLE_H ? */

#ifndef ESTRUCTS_H
#include "EStructs.h"        /* globally used generic data structures */
#define ESTRUCTS_H
#endif /* ESTRUCTS_H ? */ 

#ifndef CDVSTRING_H
#include "CDVString.h"        /* framework string implementation */
#define CDVSTRING_H
#endif /* CDVSTRING_H ? */

#ifndef CIOACCESS_H
#include "CIOAccess.h"        /* base class for recource access abstraction */
#define CIOACCESS_H
#endif /* CIOACCESS_H ? */

#ifndef CSTRINGLIST_H
#include "CStringList.h"      /* container for strings */
#define CSTRINGLIST_H
#endif /* CSTRINGLIST_H ? */

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"         /* often used gucefVFS macros */
#define GUCEFVFS_MACROS_H
#endif /* GUCEFVFS_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class EXPORT_CPP CFPCodec : public CORE::CICloneable
{
        public:
        
        CFPCodec( void );
        
        virtual ~CFPCodec();
        
        virtual void OpenPack( CORE::CIOAccess* packfile ,
                               void** packdata           ) = 0;
        
        virtual void ClosePack( CORE::CIOAccess* packfile ,
                                void** packdata           ) = 0;
        
        virtual CORE::CStringList GetFilelist( CORE::CIOAccess* packfile ,
                                               void** packdata           ) = 0;
                
        virtual CORE::CIOAccess* GetFile( CORE::CIOAccess* packfile ,
                                          const CORE::CString& file ,
                                          void** packdata           ,
                                          UInt32& errorcode         ) = 0;
        
        virtual CORE::CIOAccess* GetWriteableFile( CORE::CIOAccess* packfile ,
                                                   const CORE::CString& file ,
                                                   void** packdata           , 
                                                   UInt32& errorcode         ) = 0;
                                           
        virtual void Unload( CORE::CIOAccess* file         ,
                             const CORE::CString& filename ,
                             CORE::CIOAccess* packfile     ,
                             void** packdata               ,
                             UInt32& errorcode             ) = 0;
                                     
        virtual CORE::CString GetName( void ) = 0;
        
        /**
         *      Get the name of the type that this codec implements 
         *      support for
         *
         *      @return the name of the recource type supported by this codec
         */
        virtual CORE::CString GetTypeName( void ) const = 0;          

        virtual CORE::CString GetCopyrightEULA( void ) = 0;

        virtual const CORE::TVersion* GetVersion( void ) = 0;               
                                       
        private:         
        CFPCodec( const CFPCodec& src );            /**< should never be used */
        CFPCodec& operator=( const CFPCodec& src ); /**< should never be used */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CFPCODEC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
