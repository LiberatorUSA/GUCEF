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

#ifndef CFPCODECPLUGINREF_H
#define CFPCODECPLUGINREF_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CFPCODEC_H
#include "CFPCodec.h"
#define CFPCODEC_H
#endif /* CFPCODEC_H ? */

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

class CFPCodecPluginRef : public CFPCodec
{
        public:
        
        CFPCodecPluginRef( void );
        
        CFPCodecPluginRef( const CFPCodecPluginRef& src ); 
        
        virtual ~CFPCodecPluginRef();
        
        virtual void OpenPack( CORE::CIOAccess* packfile ,
                               void** packdata           );
        
        virtual void ClosePack( CORE::CIOAccess* packfile ,
                                void** packdata           );
        
        virtual CORE::CStringList GetFilelist( CORE::CIOAccess* packfile ,
                                               void** packdata           );
                
        virtual CORE::CIOAccess* GetFile( CORE::CIOAccess* packfile ,
                                          const CORE::CString& file ,
                                          void** packdata           ,
                                          UInt32& errorcode         );
        
        virtual CORE::CIOAccess* GetWriteableFile( CORE::CIOAccess* packfile ,
                                                   const CORE::CString& file ,
                                                   void** packdata           , 
                                                   UInt32& errorcode         );
                                           
        virtual void Unload( CORE::CIOAccess* file         ,
                             const CORE::CString& filename ,
                             CORE::CIOAccess* packfile     ,
                             void** packdata               ,
                             UInt32& errorcode             );
                                     
        virtual CORE::CString GetName( void );

        virtual CORE::CString GetCopyrightEULA( void );

        virtual const TVersion* GetVersion( void );
        
        protected:
        
        /**
         *      Decending classes should allocate a copy 
         *      of themselves and return it as the 
         *      CICloneable*
         *
         *      Note that objects that are cloned should be deleted
         *      with a call to DeleteSelf(). This ensures that
         *      memory management remains in-scope.
         */
        virtual CICloneable* CreateClone( void ) const;
        
        /**
         *      In order to keep memory management in-scope
         *      the deletion of objects should be done within
         *      the same scope as the creation.
         *      As such decending classes should implemented
         *      DestroyClonedSelf() as { delete this; }
         *      This member fuction is only called if the object
         *      was cloned and DeleteSelf() has been called. 
         */
        virtual void DestroyClonedSelf( void );                       
                                       
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CFPCODECPLUGINREF_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
