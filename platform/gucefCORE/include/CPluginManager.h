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

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#define GUCEF_CORE_CPLUGINMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"    /* MT library mutex implementation */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_MT_CILOCKABLE_H
#include "gucefMT_CILockable.h"
#define GUCEF_MT_CILOCKABLE_H
#endif /* GUCEF_MT_CILOCKABLE_H ? */

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

/*
 *      Forward declarations of framework classes used here
 */
class CPluginControl;

/*-------------------------------------------------------------------------*/

/**
 *      Generic base class for plugin managers
 */
class GUCEF_CORE_PUBLIC_CPP CPluginManager : public MT::CILockable
{
    public:

    CPluginManager( void );

    virtual ~CPluginManager();

    virtual CString GetPluginType( void ) const = 0;

    protected:
    friend class CPluginControl;
    
    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual TPluginPtr RegisterPlugin( void* modulePtr                   ,
                                       TPluginMetaDataPtr pluginMetaData ) = 0;

    virtual void UnregisterPlugin( TPluginPtr plugin ) = 0;

    private:

    CPluginManager( const CPluginManager& src );             /**< not implemented */
    CPluginManager& operator=( const CPluginManager& src );  /**< not implemented */

    private:

    MT::CMutex m_dataLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 30-09-2006 :
        - Dinand: Recoded the notification after crash wiped out the new code
- 27-11-2004 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
