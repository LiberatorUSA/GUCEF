/*
 *  gucefPRODMAN: Product management module
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_PRODMAN_CPRODUCTINFOLIST_H
#include "gucefPRODMAN_CProductInfoList.h"
#define GUCEF_PRODMAN_CPRODUCTINFOLIST_H
#endif /* GUCEF_PRODMAN_CPRODUCTINFOLIST_H ? */

#ifndef GUCEF_PRODMAN_CPRODUCTINFOLISTRETRIEVEDEVENTDATA_H
#include "gucefPRODMAN_CProductInfoListRetrievedEventData.h"
#define GUCEF_PRODMAN_CPRODUCTINFOLISTRETRIEVEDEVENTDATA_H
#endif /* GUCEF_PRODMAN_CPRODUCTINFOLISTRETRIEVEDEVENTDATA_H ? */

#include "gucefPRODMAN_CCodecBasedProductInfoListProvider.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PRODMAN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/
                  
CCodecBasedProductInfoListProvider::CCodecBasedProductInfoListProvider()
    : CProductInfoListProvider(),
      m_codecToUse( "xml" ),
      m_dataRetriever()
{GUCEF_TRACE;

    SubscribeTo( &m_dataRetriever );
}

/*-------------------------------------------------------------------------*/

CCodecBasedProductInfoListProvider::~CCodecBasedProductInfoListProvider()
{GUCEF_TRACE;

    m_dataRetriever.SetIOAccess( NULL );
}

/*-------------------------------------------------------------------------*/

void
CCodecBasedProductInfoListProvider::OnAllDataRetrieved( void )
{
    CORE::CDStoreCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry();

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
    if ( codecRegistry.TryLookup( m_codecToUse ,
                                  codec        ,
                                  false        ) )
    {
        // we have our codec, parse the data into data nodes
        CORE::CDataNode rootNode;
        if ( codec->BuildDataTree( &rootNode, m_dataRetriever.GetIOAccess() ) )
        {
            // Now try to load the product info list data
            CProductInfoListPtr productInfoList = new CProductInfoList();
            if ( productInfoList->LoadConfig( rootNode ) )
            {
                rootNode.Clear();
                
                CProductInfoListRetrievedEventData eventData( m_dataRetriever.GetURL().GetURL(), productInfoList );
                NotifyObservers( ProductInfoListRetrievedEvent, &eventData );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CCodecBasedProductInfoListProvider::RetrieveList( const CORE::CString& listLocation )
{GUCEF_TRACE;
   
    CORE::CURL& url = m_dataRetriever.GetURL();
    if ( !url.IsActive() )
    {    
        // Set a new buffer as storage
        m_dataRetriever.SetMemoryBufferAsStorage();

        url.SetURL( listLocation );
        if ( url.Activate() )
        {
            NotifyObservers( ProductInfoListRetrievalStartedEvent );
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CCodecBasedProductInfoListProvider::OnNotify( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventid  ,
                                              CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( CORE::CIURLEvents::URLAllDataRecievedEvent == eventid )
    {
        OnAllDataRetrieved();
    }
    else
    if ( CORE::CIURLEvents::URLDataRetrievalErrorEvent == eventid )
    {
        NotifyObservers( ProductInfoListRetrievalErrorEvent );
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CCodecBasedProductInfoListProvider::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PRODMAN::CCodecBasedProductInfoListProvider";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PRODMAN */
}; /* namespace GUCEF*/

/*-------------------------------------------------------------------------*/
