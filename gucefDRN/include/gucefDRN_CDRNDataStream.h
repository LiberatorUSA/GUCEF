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

#ifndef GUCEF_DRN_CDRNDATASTREAM_H
#define GUCEF_DRN_CDRNDATASTREAM_H

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

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

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

class CDRNPeerLink;

/*-------------------------------------------------------------------------*/

/**
 *  Class representing a DRN data stream
 */
class GUCEF_DRN_EXPORT_CPP CDRNDataStream : public CORE::CObservingNotifier
{
    public:

    static const CORE::CEvent DataReceivedEvent;
    static const CORE::CEvent DataTransmittedEvent;
    
    typedef CORE::TLinkedCloneableBuffer    DataTransmittedEventData; 
    typedef CORE::TLinkedCloneableBuffer    DataReceivedEventData;
    
    static void RegisterEvents( void );
    
    public:
    
    CDRNDataStream( const CORE::CString& streamName );
    
    virtual ~CDRNDataStream();
        
    void SetName( const CORE::CString& streamName );
    
    const CORE::CString& GetName( void ) const;

    bool SendData( const void* dataSource             ,
                   const UInt16 dataSize              ,
                   const bool allowUnreliable = false );

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
    
    CDRNDataStream( void );
    CDRNDataStream( const CDRNDataStream& src );
    CDRNDataStream& operator=( const CDRNDataStream& src );
    
    private:
    
    CORE::CDynamicBuffer m_sendBuffer;
    CORE::CString m_streamName;
    CDRNPeerLink* m_peerLink;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DRN_CDRNDATASTREAM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
