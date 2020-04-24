/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

#ifndef GUCEF_COM_CSTATSDCLIENT_H
#define GUCEF_COM_CSTATSDCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIMETRICSSYSTEMCLIENT_H
#include "gucefCORE_CIMetricsSystemClient.h"
#define GUCEF_CORE_CIMETRICSSYSTEMCLIENT_H
#endif /* GUCEF_CORE_CIMETRICSSYSTEMCLIENT_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_COM_MACROS_H
#include "gucefCOM_macros.h"      /* often used gucefCOM macros */
#define GUCEF_COM_MACROS_H
#endif /* GUCEF_COM_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_COM_EXPORT_CPP CStatsDClient : public CORE::CIMetricsSystemClient
{
    public:
    
    CStatsDClient( void );

    CStatsDClient( CORE::CPulseGenerator& pulseGenerator );
    
    virtual ~CStatsDClient();

    virtual bool Open( void );

    virtual bool Close( void );

    /**
     *  Increments the key, at a given frequency rate
     */
    virtual void Increment( const CString& key, const Float32 frequency = 1.0f ) const;

    /**
     *  Increments the key, at a given frequency rate
     */
    virtual void Decrement( const CString& key, const Float32 frequency = 1.0f ) const;

    /**
     *   Adjusts the specified key by a given delta, at a given frequency rate
     */
    virtual void Count( const CString& key, const Int32 delta, const Float32 frequency = 1.0f ) const;

    /**
     *  Records a gauge for the key, with a given value, at a given frequency rate
     */
    virtual void Gauge( const CString& key, const UInt32 value, const Float32 frequency = 1.0f ) const;

    /**
     *  Records a timing for a key, at a given frequency
     */
    virtual void Timing( const CString& key, const UInt32 ms, const Float32 frequency = 1.0f ) const;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot );

    void SetStatsDestination( const COMCORE::CHostAddress& dest );

    const COMCORE::CHostAddress& GetStatsDestination( void ) const;

    void SetStatsInterface( const COMCORE::CHostAddress& interface );

    const COMCORE::CHostAddress& GetStatsInterface( void ) const;

    void SetStatNamePrefix( const CString& prefix );

    const CString& GetStatNamePrefix( void ) const;

    virtual const CString& GetClassTypeName( void ) const;

    private:

    CStatsDClient( const CStatsDClient& src );             /** not implemented */
    CStatsDClient& operator=( const CStatsDClient& src );  /** not implemented */

    void 
    Transmit( const CString& key      , 
              const Int32 value       , 
              const CString& type     , 
              const Float32 frequency ) const;

    private:
    
    COMCORE::CUDPSocket m_udpSender;
    COMCORE::CHostAddress m_statsDestination;
    COMCORE::CHostAddress m_statsInterface;
    CString m_statNamePrefix;
    bool m_transmit;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CSTATSDCLIENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
