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

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_DRN_CDRNDATAGROUPPROPERTIES_H
#include "gucefDRN_CDRNDataGroupProperties.h"
#define GUCEF_DRN_CDRNDATAGROUPPROPERTIES_H
#endif /* GUCEF_DRN_CDRNDATAGROUPPROPERTIES_H ? */

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

    static const CORE::CEvent ItemAddedEvent;
    static const CORE::CEvent ItemChangedEvent;
    static const CORE::CEvent ItemRemovedEvent;
    
    struct ItemEntry
    {
        const CORE::CDynamicBuffer* id;
        const CORE::CDynamicBuffer* data;
    };
    
    typedef CORE::CTCloneableObj< struct ItemEntry >    ItemAddedEventData;
    typedef CORE::CTCloneableObj< struct ItemEntry >    ItemChangedEventData;
    typedef CORE::CTCloneableObj< struct ItemEntry >    ItemRemovedEventData;
    
    static void RegisterEvents( void );
    
    public:
    
    typedef CORE::CTSharedPtr< CDRNDataGroupProperties > CDRNDataGroupPropertiesPtr;
    
    CDRNDataGroup( const CORE::CString& groupName );
    
    virtual ~CDRNDataGroup();
        
    const CORE::CString& GetName( void ) const;
    
    void SetGroupProperties( const CDRNDataGroupPropertiesPtr& properties );
    
    CDRNDataGroupPropertiesPtr GetGroupProperties( void ) const;
    
    bool SetItem( const CORE::CDynamicBuffer& id   ,
                  const CORE::CDynamicBuffer& data ,
                  const bool addIfNotFound = true  );

    bool GetItem( const CORE::CDynamicBuffer& id ,
                  CORE::CDynamicBuffer& data     ) const;
                  
    bool HasItem( const CORE::CDynamicBuffer& id ) const;
    
    /**
     *  Attempts to locate and then remove the item with the given ID
     *  if the item is not found false is returned
     */
    bool RemoveItem( const CORE::CDynamicBuffer& id );

    UInt32 GetItemCount( void ) const;

    bool GetIDAndDataAtIndex( const UInt32 index                ,
                              const CORE::CDynamicBuffer** id   ,
                              const CORE::CDynamicBuffer** data ) const;
    
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
    
    CDRNDataGroup( void );                                /**< not possible */
    CDRNDataGroup( const CDRNDataGroup& src );            /**< not allowed at this time */
    CDRNDataGroup& operator=( const CDRNDataGroup& src ); /**< not allowed at this time */
    
    private:
    typedef std::map< CORE::CDynamicBuffer, CORE::CDynamicBuffer > TDataMap;
    
    CDRNDataGroupPropertiesPtr m_properties;
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
