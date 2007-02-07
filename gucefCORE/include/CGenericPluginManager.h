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