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

#ifndef GUCEF_MULTIPLAY_CMULTIPLAYCONTROL_H
#define GUCEF_MULTIPLAY_CMULTIPLAYCONTROL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PATCHER_CPATCHENGINE_H
#include "gucefPATCHER_CPatchEngine.h"
#define GUCEF_PATCHER_CPATCHENGINE_H
#endif /* GUCEF_PATCHER_CPATCHENGINE_H ? */

#ifndef GUCEF_DRN_CDRNNODE_H
#include "gucefDRN_CDRNNode.h"
#define GUCEF_DRN_CDRNNODE_H
#endif /* GUCEF_DRN_CDRNNODE_H ? */

#ifndef GUCEF_MULTIPLAY_CSERVERLIST_H
#include "gucefMULTIPLAY_CServerList.h"
#define GUCEF_MULTIPLAY_CSERVERLIST_H
#endif /* GUCEF_MULTIPLAY_CSERVERLIST_H ? */

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

/**
 *  If you choose to use it this class can give you all the multiplayer functionality
 *  in one sweep for true RAD of multiplayer products
 */
class GUCEF_MULTIPLAY_EXPORT_CPP CMultiPlayControl : public CORE::CIConfigurable
{
    public:
    
    CMultiPlayControl( void );
    
    virtual ~CMultiPlayControl();
    
    CServerList& GetServerList( void );
    
    const CServerList& GetServerList( void ) const;
    
    void SetServerListProvider( CServerListProvider* serverListProvider );
    
    CServerListProvider* GetServerListProvider( void );
    
    const CServerListProvider* GetServerListProvider( void ) const;

    protected:
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );    
    
    private:
    
    CMultiPlayControl( const CMultiPlayControl& src );  /**< not implemented */
    CMultiPlayControl& operator=( const CMultiPlayControl& src ); /**< not implemented */

    private:
    
    CServerList m_serverList;
    CServerListProvider* m_serverListProvider;
    PATCHER::CPatchEngine m_patchEngine;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MULTIPLAY */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MULTIPLAY_CMULTIPLAYCONTROL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-06-2007 :
       - Initial implementation.

---------------------------------------------------------------------------*/
