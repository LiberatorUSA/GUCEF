/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GUCEF_CORE_CDSTORECODECPLUGIN_H
#define GUCEF_CORE_CDSTORECODECPLUGIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CIPLUGIN_H
#include "CIPlugin.h"
#define GUCEF_CORE_CIPLUGIN_H
#endif /* GUCEF_CORE_CIPLUGIN_H ? */

#ifndef GUCEF_CORE_CDSTORECODEC_H
#include "CDStoreCodec.h"
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

/*
 *      Class that manages all the data storage codec plugins
 */
class CDStoreCodecPluginManager;

/*-------------------------------------------------------------------------*/

/**
 *      Abstract base class for an codec for storing an data tree
 *      and reading from a recource to a data tree.
 *      A codec is metadata that converts data from one type to another.
 */
class GUCEF_CORE_PUBLIC_CPP CDStoreCodecPlugin : public CDStoreCodec ,
                                                 public CIPlugin
{
    public:

    CDStoreCodecPlugin( void );
    
    CDStoreCodecPlugin( const CString& pluginfile );
    
    virtual ~CDStoreCodecPlugin();

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
     *      Returns the codec type
     *      a dat file codec might return "dat" and an xml codec "xml" ect.
     *
     *      @return the codec type
     */
    virtual CString GetTypeName( void ) const;

    /**
     *      Returns the name of the codec
     *      The author of the codec can give an codec a name that the user can
     *      obtain using this member function.
     *
     *      @return name of the codec
     */
    virtual CString GetName( void ) const;

    virtual CString GetDescription( void ) const;

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
    virtual TVersion GetVersion( void ) const;

    /**
     *      Returns path to the module
     *
     *      @return the path to the codec file
     */
    virtual CString GetModulePath( void ) const;

    virtual bool IsLoaded( void ) const;

    virtual bool Load( const CString& pluginPath );

    virtual bool Unload( void );
    
    virtual CICloneable* Clone( void ) const;

    private:
    
    CDStoreCodecPlugin( const CDStoreCodecPlugin& src ); /**< don't copy plugin objects */
    CDStoreCodecPlugin& operator=( const CDStoreCodecPlugin& src ); /**< don't copy plugin objects */

    void StoreNode( const CDataNode* n ,
                    void** filedata    ); /**< recursive node storage algorithm */

    CString _pluginfile;            /**< location of the codec plugin */
    UInt32 _id;                     /**< codec id, typicly used by manager classes */
    TDefaultFuncPtr _fptable[ 17 ]; /**< function pointer table */
    void* _sohandle;                /**< access to the so module */
    void* _plugdata;                /**< storage pointer to be used by the plugin as needed */


};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDSTORECODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 05-04-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/