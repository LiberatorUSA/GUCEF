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

#ifndef GUCEF_CORE_CGENERICPLUGIN_H
#define GUCEF_CORE_CGENERICPLUGIN_H 

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

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CIPLUGIN_H
#include "CIPlugin.h"
#define GUCEF_CORE_CIPLUGIN_H
#endif /* GUCEF_CORE_CIPLUGIN_H ? */

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

class CGenericPluginManager;

/*-------------------------------------------------------------------------*/

/**
 *  Class wrapping the generic plugin interface into an object
 *
 *  About Generic plugins:
 *  These are typicly C++ modules that link back to the GUCEF modules and
 *  uppon load integrate themselves in the framework. This allows a generic plugin
 *  to be/do just about anything but with the drawback that it has to link to the GUCEF
 *  modules and as such has a more limited lifespan as a binary plugin.
 */
class GUCEFCORE_EXPORT_CPP CGenericPlugin : public CIPlugin
{
    public:
    
    CGenericPlugin( void );
    
    virtual ~CGenericPlugin();
    
    virtual CString GetDescription( void ) const;

    virtual CString GetCopyright( void ) const;
    
    virtual TVersion GetVersion( void ) const;
    
    virtual CString GetModulePath( void ) const;
    
    virtual bool IsLoaded( void ) const;
    
    virtual bool Load( const CString& pluginPath );
    
    virtual bool Unload( void );
        
    private:
    
    static void Deinstance( void );
    
    private:    
    CGenericPlugin( const CGenericPlugin& src );        
    CGenericPlugin& operator=( const CGenericPlugin& src );
    
    private:    

    void* m_moduleHandle;
    void* m_funcPointers[ 5 ];
    CString m_modulePath;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CGENERICPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/