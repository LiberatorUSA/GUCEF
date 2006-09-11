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

#ifndef CFILEPACKAGE_H
#define CFILEPACKAGE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CDVString.h"
#include "CStringList.h"

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

class CFPCodec;
class CVFSHandle;

/*-------------------------------------------------------------------------*/

/**
 *      Internally used class to represent a file package, ie a file containing 
 *      other files. An object of this class provides access to a specific package.
 *      Combined with a codec for reading the package the files in the package can
 *      be accessed.      
 */
class CFilePackage
{
        public:
        
        CFilePackage( const CORE::CString& abspackfilepath ,
                      const CORE::CString& packfilepath    ,
                      CFPCodec* codec                      );
        
        ~CFilePackage();
        
        CORE::CString GetPackPath( void ) const;
        
        CORE::CString GetAbsolutePackPath( void ) const;
        
        CORE::CString GetPackFilename( void ) const;
        
        CVFSHandle* GetFile( const CORE::CString& file ,
                             UInt32& errorcode         );
        
        CVFSHandle* GetWriteableFile( const CORE::CString& file ,
                                      UInt32& errorcode         );
                                      
        void Unload( CVFSHandle* );                                      
                                      
        void SetMemloadSize( UInt32 bytesize );
        
        UInt32 GetMemloadSize( void ) const;                                       
                                      
        bool IsValid( void ) const;                                              
        
        private:
        CFilePackage( void );
        CFilePackage( const CFilePackage& src );
        CFilePackage& operator=( const CFilePackage& src );
        
        CORE::CIOAccess* _access;
        CORE::CString _packpath;
        CORE::CString _packabspath;
        CORE::CString _packfilename;
        CFPCodec* _codec;
        void* _packdata;
        bool _isvalid;
        CORE::CStringList files;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CFILEPACKAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
