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

#ifndef CCONFIGSTORECODECREGISTRY_H
#define CCONFIGSTORECODECREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CCONFIGSTORECODEC_H
#include "CConfigStoreCodec.h"  /* base class for config codec implementations */
#define CCONFIGSTORECODEC_H
#endif /* CCONFIGSTORECODEC_H ? */

#ifndef CREGISTRY_H
#include "CRegistry.h"         /* generic registry class */
#define CREGISTRY_H
#endif /* CTREGISTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class EXPORT_CPP CConfigStoreCodecRegistry
{
        public:
        
        CConfigStoreCodecRegistry* Instance( void ); 
        
        /**
         *      Attempts to obtain a cloned codec object of the codec
         *      with the name given. If the requested object cannot be found
         *      NULL will be returned.
         *
         *      @param typenamestr Type name of the object for which you want to obtain a clone
         *      @return A clone of the object of the type given. Can be NULL if the type was not found.
         */
        CConfigStoreCodec* GetClone( const CString& typenamestr );
        
        /**
         *      Clones the given object and registers it's typename
         *      to make it available for calls to GetClone()
         */
        void Register( const CConfigStoreCodec* object );
        
        /**
         *      Unregisters the object with the given typename.
         *      It will no longer be available for cloning with
         *      GetClone().
         */
        void Unregister( const CString& typenamestr );                  
        
        private:
        
        CConfigStoreCodecRegistry( void );
        CConfigStoreCodecRegistry( const CConfigStoreCodecRegistry& src );
        CConfigStoreCodecRegistry& operator=( const CConfigStoreCodecRegistry& src );
        ~CConfigStoreCodecRegistry();
        
        CRegistry _registry;
        
        static CConfigStoreCodecRegistry* _instance;
        static MT::CMutex _mutex;
};

/*-------------------------------------------------------------------------*/

#endif /* CCONFIGSTORECODECREGISTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-02-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
