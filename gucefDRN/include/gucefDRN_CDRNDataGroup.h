/*
 *  gucefDRN: GUCEF module providing RAD networking trough data replication
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

#ifndef GUCEF_DRN_CDRNDATAGROUP_H
#define GUCEF_DRN_CDRNDATAGROUP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <map>

#ifndef GUCEF_CORE_CISTREAMABLE_H
#include "CIStreamable.h"
#define GUCEF_CORE_CISTREAMABLE_H
#endif /* GUCEF_CORE_CISTREAMABLE_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */ 

#ifndef GUCEF_DRN_MACROS_H
#include "gucefDRN_macros.h"
#define GUCEF_DRN_MACROS_H
#endif /* GUCEF_DRN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace DRN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Data holder for a DRN data group.
 *  It groups data that can be processed according to the DRN settings of this
 *  class. 
 *  It is capable of handling GUCEF::CORE::CStreamerEvents::StreamEvent if so desired
 *  Depending on the settings streamed data can be processed as part of a DRN group
 *  Note that for StreamEvent to be accepted it will have to have an ID.
 */
class GUCEF_DRN_EXPORT_CPP CDRNDataGroup : public CORE::CObservingNotifier
{
    public:

    static const CORE::CEvent ItemChanged;
    
    static void RegisterEvents( void );
    
    public:
    
    CDRNDataGroup( void );
    
    CDRNDataGroup( const CDRNDataGroup& src );
    
    virtual ~CDRNDataGroup();
    
    CDRNDataGroup& operator=( const CDRNDataGroup& src );
    
    void SetName( const CORE::CString& groupName );
    
    const CORE::CString& GetName( void ) const;
    
    /**
     *  Default is false
     */
    void SetEmitEntireGroupOnChange( const bool emitEntireGroup );
    
    bool GetEmitEntireGroupOnChange( void ) const;
    
    /**
     *  Default is true
     */
    void SetAcceptNewStreamerItems( const bool acceptNewSItems );
    
    bool GetAcceptNewStreamerItems( void ) const;
    
    /**
     *  Default is true
     */
    void SetAcceptUpdatesFromStreamers( const bool acceptStreamerUpdates );
    
    bool GetAcceptUpdatesFromStreamers( void ) const;
    
    bool SetItem( const CORE::CDynamicBuffer& id   ,
                  const CORE::CDynamicBuffer& data ,
                  const bool addIfNotFound = true  );

    bool GetItem( const CORE::CDynamicBuffer& id ,
                  CORE::CDynamicBuffer& data     ) const;
                  
    bool HasItem( const CORE::CDynamicBuffer& id ) const;

    protected:
    
    /**
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );
    
    private:
    typedef std::map< CORE::CDynamicBuffer, CORE::CDynamicBuffer > TDataMap;
    
    bool m_emitAsGroup;
    bool m_acceptNewSItems;
    bool m_acceptStreamerUpdates;
    CORE::CString m_groupName;
    TDataMap m_dataMap;
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DRN_CDRNDATAGROUP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
