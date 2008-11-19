/*
 *  gucefMULTIPLAY: GUCEF module providing multiplayer RAD functionality
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

#ifndef GUCEF_MULTIPLAY_CSERVERLISTPROVIDER_H
#define GUCEF_MULTIPLAY_CSERVERLISTPROVIDER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_MULTIPLAY_MACROS_H
#include "gucefMULTIPLAY_macros.h"
#define GUCE_MULTIPLAY_MACROS_H
#endif /* GUCEF_MULTIPLAY_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MULTIPLAY {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_MULTIPLAY_EXPORT_CPP CServerListProvider : public CORE::CObservingNotifier
{
    public:
    
    struct SServerInfo
    {
        CORE::CString name;                /**< name of the server */
        COMCORE::CHostAddress hostAddress; /**< remote address of the server, either hostname or an IP */
        CORE::CValueList extra;            /**< additional info regarding the server entry */
    };
    typedef struct SServerInfo TServerInfo;
    
    static const CORE::CEvent ServerListProviderStartEvent;    /**< event indicating that the list provider has started */
    static const CORE::CEvent ServerInfoEvent;                 /**< event indicating that a list entry has been obtained, event data is included */
    static const CORE::CEvent ServerListProviderAbortEvent;    /**< event indicating that the list provider operation was aborted */
    static const CORE::CEvent ServerListProviderErrorEvent;    /**< event indicating that the list provider operation encountered an error */
    static const CORE::CEvent ServerListProviderFinisedEvent;  /**< event indicating that the list provider operation is finished */
    
    typedef CORE::CTCloneableObj< TServerInfo > ServerInfoEventData;   
   
    static void RegisterEvents( void );

    public:
    
    CServerListProvider( void );
    
    CServerListProvider( const CServerListProvider& src );
    
    virtual ~CServerListProvider();
    
    CServerListProvider& operator=( const CServerListProvider& src );
    
    virtual bool Start( const CORE::CValueList& params ) = 0;
    
    virtual bool IsBusy( void ) = 0;
    
    virtual void Stop( void ) = 0;
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MULTIPLAY */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MULTIPLAY_CSERVERLISTPROVIDER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-04-2007 :
       - Initial implementation.

---------------------------------------------------------------------------*/
