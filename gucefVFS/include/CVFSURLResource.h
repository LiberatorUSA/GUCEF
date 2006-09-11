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

#ifndef CVFSURLRESOURCE_H
#define CVFSURLRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CURLRESOURCE_H
#include "CURLResource.h"        /* base class for url recource access */
#define CURLRESOURCE_H
#endif /* CURLRESOURCE_H ? */

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"     /* often used gucefVFS macros */
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

class CVFSHandle;

/*-------------------------------------------------------------------------*/

/**
 *      Base class for URL recources.
 *      Certain handlers may want to store additional data then just the
 *      CIOAccess pointer. Thats where this class comes in. Handlers can
 *      use a class that decends from this class to store additional
 *      information.
 */
class EXPORT_CPP CVFSURLResource : public CORE::CURLResource
{
        public:             
        
        /**
         *      Initialzes the object with the given vfs handle
         */
        CVFSURLResource( CVFSHandle* handle );        
        
        /**
         *      Does nothing atm.
         */
        ~CVFSURLResource();
        
        /**
         *      Returns a pointer to the VFS recource handle
         */
        CVFSHandle* GetVFSHandle( void ) const;             
        
        private:
        CVFSURLResource( void );
        CVFSURLResource( const CVFSURLResource& src );            /**< we do not want any rogue copies out there */       
        
        CVFSHandle* _handle;     /**< pointer to the vfs handle */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CVFSURLRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-04-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
