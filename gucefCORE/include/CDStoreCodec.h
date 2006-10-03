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

#ifndef GUCEF_CORE_CDSTORECODEC_H
#define GUCEF_CORE_CDSTORECODEC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"     /* abstract base class for cloneable objects */
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"        /* globally used generic data structures */
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */ 

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"        /* framework string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

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

class CDataNode;
class CIOAccess;

/*-------------------------------------------------------------------------*/

/**
 *      Abstract base class for an codec for storing an data tree 
 *      and reading from a recource to a data tree.
 *      A codec is metadata that converts data from one type to another.
 */
class GUCEFCORE_EXPORT_CPP CDStoreCodec : public CICloneable 
{
        public:
        
        CDStoreCodec( void );

        virtual ~CDStoreCodec();
        
        /**
         *      Wheter or not the codec is currently valid
         *
         *      @return validity of the codec
         */
        virtual bool IsValid( void ) const = 0;
        
        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec metadata
         *
         *      @param tree the data tree you wish to store
         *      @param filename path and name of the file you wish to store the data in.
         *      @return wheter storing the tree was successfull
         */
        virtual bool StoreDataTree( const CDataNode* tree   ,
                                    const CString& filename ) = 0;

        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec metadata
         *
         *      @param tree the data tree you wish to store
         *      @param file access to the recource to which the data will be written
         *      @return wheter storing the tree was successfull
         */
        virtual bool StoreDataTree( const CDataNode* tree   ,
                                    CIOAccess* file         ) = 0;
                                    
                                    
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
                                    const CString& filename ) = 0;
                                    
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
                                    CIOAccess* file     ) = 0;
        
        /**
         *      Returns the name of the codec
         *      The author of the codec can give an codec a name that the user can
         *      obtain using this member function.
         *
         *      @return name of the codec
         */
        virtual CString GetName( void ) const = 0;
        
        /**
         *      Get the name of the type that this codec implements 
         *      support for
         *
         *      @return the name of the recource type supported by this codec
         */
        virtual CString GetTypeName( void ) const = 0;
        
        /**
         *      Returns the copyright of the codec 
         *      The author of the codec can give an codec a copyright notice that the user can
         *      obtain using this member function.
         *
         *      @return copyright notice of the codec
         */
        virtual CString GetCopyright( void ) const = 0;
        
        /**
         *      Returns the codec version
         *
         *      @return the version of the codec
         */
        virtual const TVersion* GetVersion( void ) const = 0;
        
        private:
        CDStoreCodec( const CDStoreCodec& src );             /**< no sense in an copied codec */
        CDStoreCodec& operator=( const CDStoreCodec& src );  /**< no sense in an copied codec */                 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDSTORECODEC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-04-2005 :
        - Added overloaded BuildDataTree() that accepts an CIOAccess pointer
          as data source.
- 05-04-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
