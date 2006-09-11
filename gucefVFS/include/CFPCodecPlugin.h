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

#ifndef CFPCODECPLUGIN_H
#define CFPCODECPLUGIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CIOAccess.h"
#include "CStringList.h"
#include "EStructs.h"

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

class EXPORT_CPP CFPCodecPlugin
{
        public:
        
        CFPCodecPlugin( const CORE::CString& codecfile );
        
        ~CFPCodecPlugin();
        
        void OpenPack( CORE::CIOAccess* packfile ,
                       void** packdata           );
        
        void ClosePack( CORE::CIOAccess* packfile ,
                        void** packdata           );
        
        CORE::CStringList GetFilelist( CORE::CIOAccess* packfile ,
                                       void** packdata           );
                
        CORE::CIOAccess* GetFile( CORE::CIOAccess* packfile ,
                                  const CORE::CString& file ,
                                  void** packdata           ,
                                  UInt32& errorcode         );
        
        CORE::CIOAccess* GetWriteableFile( CORE::CIOAccess* packfile ,
                                           const CORE::CString& file ,
                                           void** packdata           , 
                                           UInt32& errorcode         );
                                           
        void Unload( CORE::CIOAccess* file         ,
                     const CORE::CString& filename ,
                     CORE::CIOAccess* packfile     ,
                     void** packdata               ,
                     UInt32& errorcode             );
                                     
        CORE::CString GetName( void );

        CORE::CString GetCopyrightEULA( void );

        const CORE::TVersion* GetVersion( void );
        
        bool IsValid( void ) const;
        
        UInt32 GetCodecID( void ) const;
        
        CORE::CString GetLocation( void ) const;
        
        private:
        friend class CFPCodecPluginManager;
        
        void SetCodecID( UInt32 id );
                                       
        private:         
        CFPCodecPlugin( void );                                 /**< should never be used */   
        CFPCodecPlugin( const CFPCodecPlugin& src );            /**< should never be used */
        CFPCodecPlugin& operator=( const CFPCodecPlugin& src ); /**< should never be used */

        UInt32 _id;           /**< codec id, typicly used by manager classes */
        void* _fptable[ 12 ]; /**< function pointer table */
        void* _sohandle;      /**< access to the so module */
        void* _plugdata;      /**< storage pointer to be used by the plugin as needed */
        CORE::CString _path;  /**< path to the plugin module */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CFPCODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
