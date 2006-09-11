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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CVFSHandle.h"

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CVFSHandle::CVFSHandle( void )
{TRACE;
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CVFSHandle::CVFSHandle( const CVFSHandle& src )
{TRACE;
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CVFSHandle::CVFSHandle( CORE::CIOAccess* fileaccess   ,
                        const CORE::CString& filename ,
                        bool packed                   ,
                        UInt32 index                  )
        : _fileaccess( fileaccess ) ,
          _filename( filename )     ,
          _packed( packed )         ,
          _index( index )          
{TRACE;
}                        

/*-------------------------------------------------------------------------*/
        
CVFSHandle& 
CVFSHandle::operator=( const CVFSHandle& src )
{TRACE;
        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/

CVFSHandle::~CVFSHandle()
{TRACE;
        /* dont do anything here, this is just a storage / encapsulation class */
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CVFSHandle::GetFilename( void ) const
{TRACE;
        return _filename;
}

/*-------------------------------------------------------------------------*/

bool
CVFSHandle::IsPacked( void ) const
{TRACE;
        return _packed;
}

/*-------------------------------------------------------------------------*/

CORE::CIOAccess* 
CVFSHandle::GetAccess( void ) const
{TRACE;
        return _fileaccess;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
