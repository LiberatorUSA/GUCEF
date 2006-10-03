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

#ifndef GUCEF_CORE_CDSTORECODECPLUGINREF_H
#define GUCEF_CORE_CDSTORECODECPLUGINREF_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDSTORECODEC_H
#include "CDStoreCodec.h"       /* abstract data storage codec base class */
#define GUCEF_CORE_CDSTORECODEC_H
#endif /* GUCEF_CORE_CDSTORECODEC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CDStoreCodecPlugin;

/*-------------------------------------------------------------------------*/

class GUCEFCORE_EXPORT_CPP CDStoreCodecPluginRef : public CDStoreCodec 
{
        public:
        
        CDStoreCodecPluginRef( void );
        
        CDStoreCodecPluginRef( const CDStoreCodecPluginRef& src );
        
        virtual ~CDStoreCodecPluginRef();
        
        /**
         *      Wheter or not the codec is currently valid
         *
         *      @return validity of the codec
         */
        virtual bool IsValid( void ) const;
        
        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec metadata
         *
         *      @param tree the data tree you wish to store
         *      @param filename path and name of the file you wish to store the data in.
         *      @return wheter storing the tree was successfull
         */
        virtual bool StoreDataTree( const CDataNode* tree   ,
                                    const CString& filename );

        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec metadata
         *
         *      @param tree the data tree you wish to store
         *      @param file access to the recource to which the data will be written
         *      @return wheter storing the tree was successfull
         */
        virtual bool StoreDataTree( const CDataNode* tree   ,
                                    CIOAccess* file         );
                                    
        /**
         *      Attempts to load data from the given file to the 
         *      root node given. The root data will be replaced 
         *      and any children the node may already have will be deleted.
         *
         *      @param treeroot pointer to the node that is to act as root of the data tree
         *      @param filename path and name of the file from which the data tree information should be loaded
         *      @return whether building the tree from the given file was successfull.
         */                                    
        virtual bool BuildDataTree( CDataNode* treeroot     ,
                                    const CString& filename );
                                    
        /**
         *      Attempts to load data from the given file to the 
         *      root node given. The root data will be replaced 
         *      and any children the node may already have will be deleted.
         *
         *      @param treeroot pointer to the node that is to act as root of the data tree
         *      @param file media independant access to the data source from which the data tree information should be loaded
         *      @return whether building the tree from the given file was successfull.
         */                                    
        virtual bool BuildDataTree( CDataNode* treeroot ,
                                    CIOAccess* file     );                                                                        
        
        /**
         *      Returns the name of the codec
         *      The author of the codec can give an codec a name that the user can
         *      obtain using this member function.
         *
         *      @return name of the codec
         */
        virtual CString GetName( void ) const;
        
        virtual CString GetTypeName( void ) const;
        
        /**
         *      Returns the copyright of the codec 
         *      The author of the codec can give an codec a copyright notice that the user can
         *      obtain using this member function.
         *
         *      @return copyright notice of the codec
         */
        virtual CString GetCopyright( void ) const;
        
        /**
         *      Returns the codec version
         *
         *      @return the version of the codec
         */
        virtual const TVersion* GetVersion( void ) const;
        
        virtual CICloneable* Clone( void ) const;
        
        private:
        friend class CDStoreCodecPlugin;
        
        CDStoreCodecPlugin* _plugin;                 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDSTORECODECPLUGINREF_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 05-04-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/