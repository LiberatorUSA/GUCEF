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
 
#ifndef GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H 
#define GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 
 
#ifndef GUCEF_MT_CMUTEX_H
#include "CMutex.h"             /* MT library mutex implementation */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"   /* often used gucefCORE macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */ 
 
#ifndef GUCEF_CORE_CPLUGINMANAGER_H 
#include "CPluginManager.h"    /* base class for plugin managers */
#define GUCEF_CORE_CPLUGINMANAGER_H
#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"      /* dynamic array implementation */
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECPLUGIN_H
#include "CDStoreCodecPlugin.h" 
#define GUCEF_CORE_CDSTORECODECPLUGIN_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGIN_H */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"             /* abstract base class interface for configurable objects */
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

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
 
class GUCEFCORE_EXPORT_CPP CDStoreCodecPluginManager : public CPluginManager ,
                                                       public CIConfigurable
{
    public:
    
    static CDStoreCodecPluginManager* Instance( void );
    
    CDStoreCodecPlugin* LoadCodecPlugin( const CString& filename );
    
    void UnloadCodecPlugin( CDStoreCodecPlugin* plugin );
    
    CDStoreCodecPlugin* GetCodec( const CString& codectype ) const;
    
    virtual void LoadAll( void );
    
    virtual void LoadAll( const CString& pluginDir );
    
    virtual void UnloadAll( void );
    
    /**
     *      Attempts to store the given tree in the file
     *      given according to the method of the codec metadata
     *
     *      @param tree the data tree you wish to store
     *      @return wheter storing the tree was successfull
     */
    virtual bool SaveConfig( CDataNode& tree );
                                
    /**
     *      Attempts to load data from the given file to the 
     *      root node given. The root data will be replaced 
     *      and any children the node may already have will be deleted.
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @return whether building the tree from the given file was successfull.
     */                                    
    virtual bool LoadConfig( const CDataNode& treeroot );        
    
    protected:
    
    virtual void OnSetPluginDir( const CString& path );        

    private:
    friend class CGUCEFCOREModule;
    
    static void Deinstance( void );
    
    private:
    
    static MT::CMutex _datalock;
    static CDStoreCodecPluginManager* _instance;
    
    bool IsPluginLoaded( const CString& path );
    
    CDStoreCodecPluginManager( void );
    CDStoreCodecPluginManager( const CDStoreCodecPluginManager& src );        
    ~CDStoreCodecPluginManager();
    CDStoreCodecPluginManager& operator=( const CDStoreCodecPluginManager& src );
    
    CDynamicArray _codecs;        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-09-2005 :
        - Added check that makes sure the same plugin module is not loaded twice
- 04-04-2005 :
        - Initial version.

-----------------------------------------------------------------------------*/