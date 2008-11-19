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

#ifndef GUCEF_CORE_CGENERICPLUGINMANAGER_H
#define GUCEF_CORE_CGENERICPLUGINMANAGER_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#include "CPluginManager.h"
#define GUCEF_CORE_CPLUGINMANAGER_H
#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

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

class CGenericPlugin;

/*-------------------------------------------------------------------------*/

/**
 *  Plugin manager for generic plugins
 *
 *  About Generic plugins:
 *  These are typicly C++ modules that link back to the GUCEF modules and
 *  uppon load integrate themselves in the framework. This allows a generic plugin
 *  to be/do just about anything but with the drawback that it has to link to the GUCEF
 *  modules and as such has a more limited lifespan as a C-interface plugin.
 */
class GUCEFCORE_EXPORT_CPP CGenericPluginManager : public CPluginManager
{
    public:    
    
    static CGenericPluginManager* Instance( void );
    
    virtual void LoadAll( void );
    
    virtual void UnloadAll( void );
    
    virtual bool IsLoaded( const CString& pluginPath );
    
    virtual bool Load( const CString& pluginPath );
    
    virtual bool Unload( const CString& pluginPath );
    
    virtual ~CGenericPluginManager();
    
    private:
    friend class CGUCEFCOREModule;
    
    static void Deinstance( void );
    
    private:
    CGenericPluginManager( void );
    CGenericPluginManager( const CGenericPluginManager& src );        
    CGenericPluginManager& operator=( const CGenericPluginManager& src );
    
    private:    
    typedef std::vector< CGenericPlugin* > TPluginList;
    
    TPluginList m_pluginList;
    
    static CGenericPluginManager* m_instance;
};


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CGENERICPLUGINMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/