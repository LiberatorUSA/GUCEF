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

#ifndef GUCEF_CORE_CONFIGSTORE_H
#define GUCEF_CORE_CONFIGSTORE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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
 *      Forward declaration of the abstract base for configurable items.
 */
class CIConfigurable;
class CDataNode;

/*-------------------------------------------------------------------------*/

/**
 *      Centralized configuration control.
 *      Allows you load and save the config of all configureable items that
 *      have global config usage enabled.
 */
class GUCEF_CORE_PUBLIC_CPP CConfigStore : public CTSGNotifier
{
    public:

    static const CEvent GlobalConfigLoadStartingEvent;
    static const CEvent GlobalConfigLoadCompletedEvent;
    static const CEvent GlobalConfigLoadFailedEvent;

    static void RegisterEvents( void );

    void SetConfigFile( const CString& filepath );

    CString GetConfigFile( void ) const;

    bool SaveConfig( const CString& name  ,
                     bool preserve = true );

    bool LoadConfig( CDataNode* loadedConfig = GUCEF_NULL );

    void SetCodec( const CString& codectype );

    CString GetCodec( void ) const;

    bool IsGlobalConfigLoadInProgress( void ) const;

    private:

    friend class CIConfigurable;

    void Register( CIConfigurable* configobj );

    bool Unregister( CIConfigurable* configobj );

    private:
    friend class CCoreGlobal;

    CConfigStore( void );

    virtual ~CConfigStore();

    private:

    CConfigStore( const CConfigStore& src );             /**< not implemented */
    CConfigStore& operator=( const CConfigStore& src );  /**< not implemented */

    private:

    typedef std::set< CIConfigurable* > TConfigurableSet;

    CString _codectype;
    CString _configfile;
    TConfigurableSet m_configureables;
    TConfigurableSet m_newConfigureables;
    bool m_isBusyLoadingConfig;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
