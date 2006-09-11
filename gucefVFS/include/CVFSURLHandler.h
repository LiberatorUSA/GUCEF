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

#ifndef CVFSURLHANDLER_H
#define CVFSURLHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CURLHANDLER_H
#include "CURLHandler.h"             /* gucefCORE url handler base class */
#define GUCEF_CORE_CURLHANDLER_H
#endif /* GUCEF_CORE_CURLHANDLER_H ? */

#ifndef GUCEF_VFS_MACROS_H
#include "gucefVFS_macros.h"         /* often used gucefVFS macros */
#define GUCEF_VFS_MACROS_H
#endif /* GUCEF_VFS_MACROS_H ? */ 

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

class CVFS;

/*-------------------------------------------------------------------------*/

/**
 *      VFS URL handling implementation.
 *      for example: vfs://foo/bar.dat
 */
class EXPORT_CPP CVFSURLHandler : public CORE::CURLHandler
{
        public:
        typedef CORE::CURLHandler::TDataHandlerSet TDataHandlerSet;
        
        /**
         *      Doesnt do anything special atm.
         */        
        CVFSURLHandler( void );
        
        /**
         *      Doesnt do anything special atm.
         */        
        CVFSURLHandler( const CVFSURLHandler& src );
        
        /**
         *      Doesnt do anything special atm.
         */
        virtual ~CVFSURLHandler();
        
        /**
         *      Doesnt do anything special atm.
         */        
        CVFSURLHandler& operator=( const CVFSURLHandler& src );

        virtual bool Activate( CORE::CURL& url               ,
                               TDataHandlerSet& dataHandlers );
        
        virtual void Deactivate( CORE::CURL& url               ,
                                 TDataHandlerSet& dataHandlers );
                         
        virtual bool IsActive( const CORE::CURL& url ) const;        

        virtual CORE::CICloneable* Clone( void ) const;
        
        static void Register( void );
        
        static void Unregister( void );        
        
        private:
        
        CVFS* m_vfs;        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CVFSURLHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-04-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/