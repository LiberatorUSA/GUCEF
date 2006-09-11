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

#ifndef CFPCODECREGISTRY_H
#define CFPCODECREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"               /* gucefMT mutex implementation */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef CTREGISTRY_H
#include "CTRegistry.h"         /* registry template for multi-threaded apps */
#define CTREGISTRY_H
#endif /* CTREGISTRY_H ? */

#ifndef GUCEFVFS_MACROS_H
#include "gucefVFS_macros.h"     /* gucefVFS Library macros */
#define GUCEFVFS_MACROS_H
#endif /* GUCEFVFS_MACROS_H ? */

#ifndef CFPCODEC_H
#include "CFPCodec.h"            /* base class for file pack codecs */
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
 
template EXPORT_CPP CORE::CTRegistry< CFPCodec >;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CFPCodecRegistry : public CORE::CTRegistry< CFPCodec >
{
        public:
        
        static CFPCodecRegistry* Instance( void );
        
        protected:
        virtual void LockData( void ) const;
        virtual void UnlockData( void ) const;        
        
        private:
        friend class CGUCEFVFSModule;
        
        static void Deinstance( void );
        
        private:
        CFPCodecRegistry( void );
        CFPCodecRegistry( const CFPCodecRegistry& src );        
        virtual ~CFPCodecRegistry();
        CFPCodecRegistry& operator=( const CFPCodecRegistry& src );
        
        static CFPCodecRegistry* _instance;
        static MT::CMutex _datalock;        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CFPCODECREGISTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-04-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
