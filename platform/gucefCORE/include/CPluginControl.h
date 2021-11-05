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

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#define GUCEF_CORE_CPLUGINCONTROL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIPLUGINLOADLOGIC_H
#include "gucefCORE_CIPluginLoadLogic.h"
#define GUCEF_CORE_CIPLUGINLOADLOGIC_H
#endif /* GUCEF_CORE_CIPLUGINLOADLOGIC_H ? */

#ifndef GUCEF_CORE_CPLUGINGROUP_H
#include "gucefCORE_CPluginGroup.h"
#define GUCEF_CORE_CPLUGINGROUP_H
#endif /* GUCEF_CORE_CPLUGINGROUP_H ? */

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CPLUGINMETADATA_H
#include "gucefCORE_CPluginMetaData.h"
#define GUCEF_CORE_CPLUGINMETADATA_H
#endif /* GUCEF_CORE_CPLUGINMETADATA_H ? */

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
 *      Forward declarations of framework classes used here
 */
class CPluginManager;

/*-------------------------------------------------------------------------*/

/**
 *  Global framework plugin control center
 *
 *  All plugin managers derived from CPluginManager automaticly register here.
 *  By seperating the logic of loading modules from the specifics of module interfaces
 *  we can support any plugin interface as long as a plugin manager is registered capable
 *  of handling the given plugin's interface.
 */
class GUCEF_CORE_PUBLIC_CPP CPluginControl : public CTSGNotifier    ,
                                             public CIConfigurable
{
    public:

    typedef CString::StringSet  TStringSet;

    static const CEvent PluginLoadedEvent;
    static const CEvent PluginUnloadedEvent;

    static void RegisterEvents( void );

    public:

    typedef std::vector< CString > TStringVector;

    bool LoadAll( void );

    bool LoadAllPluginsOfType( const CString& pluginTypeToLoad );

    bool LoadAllPluginsOfTypeInGroup( const CString& pluginTypeToLoad ,
                                      const CString& groupName        );

    bool LoadPluginGroup( const CString& groupName );

    bool UnloadAllPluginsOfTypeInGroup( const CString& pluginTypeToLoad ,
                                        const CString& groupName        );

    bool UnloadAllPluginsOfType( const CString& pluginTypeToUnload );

    bool UnloadPluginGroup( const CString& groupName );

    bool UnloadPlugin( const CString& groupName  ,
                       const CString& moduleName );

    bool UnloadAll( void );

    void GetAvailablePluginGroups( TStringSet& groupNames ) const;

    void GetAvailablePluginsForGroup( const CString& groupName , 
                                      TStringSet& modules      ) const;

    void ClearMetaDataFromGroup( const CString& groupName );

    bool AddPluginMetaData( const CIPluginMetaData& pluginMetaData ,
                            const CString& groupName               ,
                            bool loadImmediatly                    );

    bool AddAllPluginsFromDir( const CString& pluginDir ,
                               const CString& groupName ,
                               bool loadImmediatly      );

    /**
     *  Utility function: defines limited plugin meta data from the limited information
     *  available and then performs a trial and error load.
     *  If you have more information regarding the plugin you should use
     *      AddPluginMetaData() instead and pass that information
     */
    bool AddPluginFromDir( const CString& pluginPath ,
                           const CString& groupName  ,
                           bool loadImmediatly       );

    /**
     *  Utility function: defines limited plugin meta data from the limited information
     *  available and then performs a trial and error load.
     *  If you have more information regarding the plugin you should use
     *      AddPluginMetaData() instead and pass that information
     *  Note that the directories set as plugin dirs will be used as the directories to
     *  search. See AddPluginDir()
     */
    bool SearchForPluginInPluginDirs( const CString& pluginFileName ,
                                      const CString& groupName      ,
                                      bool loadImmediatly           );

    /**
     *  Adds a plugin root dir to the dirs that will be used to load
     *  modules from if the plugin metadata does not already specify a directory to use
     *  Variables in the path are automatically resolved.
     */
    void AddPluginDir( const CString& path );

    /**
     *  Removes a plugin root dir to the dirs that will be used to load
     *  modules from if the plugin metadata does not already specify a directory to use
     *  Variables in the path are automatically resolved.
     */
    void RemovePluginDir( const CString& path );

    /**
     *  Gets a list of all supported plugin types.
     *  The types of plugins supported is determined by the types of
     *  plugin managers registered.
     */
    void GetSupportedPluginTypes( TStringVector& supportedTypes ) const;

    /**
     *  Registers the given loader with the plugin controller.
     *  Note that ownership is not assumed and the given object is assumed to be
     *  alive for the duration of its registration.
     */
    void RegisterPluginLoadLogic( CIPluginLoadLogic* pluginLoaderLogic );

    /**
     *  Unregisters the given loader with the plugin controller.
     */
    void UnregisterPluginLoadLogic( const CString& loaderLogicTypeName );

    /**
     *  Sets the default loading logic type to be used for loading modules.
     *  Note that if the plugin metadata specifies loading logic type then that
     *  type will be used instead of the default.
     */
    void SetDefaultPluginLoadLogicType( const CString& loaderLogicTypeName );

    /**
     *  Gets the default loading logic type to be used for loading modules.
     */
    CString GetDefaultPluginLoadLogicType( void ) const;

    /**
     *  Attempts to store the given tree in the file
     *  given according to the method of the codec metadata
     *
     *  @param tree the data tree you wish to store
     *  @return wheter storing the tree was successfull
     */
    virtual bool SaveConfig( CDataNode& tree ) const;

    /**
     *  Attempts to load data from the given file to the
     *  root node given. The root data will be replaced
     *  and any children the node may already have will be deleted.
     *
     *  @param treeroot pointer to the node that is to act as root of the data tree
     *  @return whether building the tree from the given file was successfull.
     */
    virtual bool LoadConfig( const CDataNode& treeroot );

    virtual const CString& GetClassTypeName( void ) const;

    protected:

    virtual void OnPumpedNotify( CNotifier* notifier                 ,
                                 const CEvent& eventid               ,
                                 CICloneable* eventdata = GUCEF_NULL ) GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CPluginManager;

    void Register( CPluginManager* pman );

    void Unregister( CPluginManager* pman );

    private:
    friend class CCoreGlobal;

    CPluginControl( void );

    ~CPluginControl();

    private:

    typedef std::map< CString, CIPluginLoadLogic* > TPluginLoadLogicMap;
    typedef std::map< CString, CPluginGroup > TPluginGroupMap;
    typedef std::set< CPluginManager* > TPluginManagerSet;    

    CPluginControl( const CPluginControl& src );

    CPluginControl& operator=( const CPluginControl& src );

    bool LoadPlugin( TPluginMetaDataPtr& pluginMetaData ,
                     CPluginGroup& pluginGroup          ,
                     const CString& groupName           );

    bool UnloadPlugin( TPluginPtr& plugin        ,
                       CPluginGroup& pluginGroup ,
                       const CString& groupName  );

    CPluginManager* GetPluginManagerForType( const CString& pluginType );

    TPluginLoadLogicMap m_pluginLoadLogicProviders;
    CString m_defaultPluginLoadLogicType;
    TPluginGroupMap m_pluginGroups;
    TStringSet m_rootDirs;
    TPluginManagerSet m_pluginManagers;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Initial implementation

-----------------------------------------------------------------------------*/
