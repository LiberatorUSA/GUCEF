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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_DRN_H
#include "gucefDRN.h"           /* GUCEF DRN library API */
#define GUCEF_DRN_H
#endif /* GUCEF_DRN_H ? */

#ifdef GUCEF_MSWIN_BUILD
  #include <windows.h>
  #undef min
  #undef max
#else
  #include <assert.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define DRN_PEERCOMM_PORT   50456

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MSWIN_BUILD
  #define DebugBreak() assert( 0 )
#endif

/*-------------------------------------------------------------------------*/

#define ERRORHERE { GUCEF_ERROR_LOG( 0, GUCEF::CORE::CString( "Test failed @ " ) + GUCEF::CORE::CString( __FILE__ ) + GUCEF::CORE::Int32ToString( __LINE__ ) ); \
                    printf( "Test failed @ %s(%d)\n", __FILE__, __LINE__ );                                                                                      \
                    DebugBreak();                                                                                                                                \
                  }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CTestPeerValidator : public DRN::CIDRNPeerValidator
{
    public:

    virtual bool IsPeerAddressValid( const TIPAddress& address     ,
                                     const CORE::CString& hostName ) const
    {
        return ( hostName == "localhost" ) || ( hostName == "127.0.0.1" );
    }
    
    virtual bool IsPeerLoginValid( const CORE::CString& accountName ,
                                   const CORE::CString& password    ) const
    {
        return  ( ( accountName == "ValidPeer" ) && ( password == "Peer" ) );
    }

    virtual bool IsPeerServiceValid( const CORE::CString& serviceName     ,
                                     const CORE::TVersion& serviceVersion ) const
    {
        if ( serviceName == "TestPeerToPeer" )
        {
            CORE::TVersion correctVersion;
            correctVersion.mayor = 1;
            correctVersion.minor = 0;
            correctVersion.patch = 0;
            correctVersion.release = 0;
            
            return ( memcmp( &serviceVersion, &correctVersion, sizeof( CORE::TVersion ) ) == 0 );
        }        
        return false;
    }    
};

/*-------------------------------------------------------------------------*/

class CTestPeerToPeerSubSystem : public CORE::CGUCEFAppSubSystem
{
    private:

    CTestPeerValidator testValidator;        
    DRN::CDRNNode nodeA;
    DRN::CDRNNode nodeB; 

    public:
    
    CTestPeerToPeerSubSystem( void )    
        : CGUCEFAppSubSystem( true )
    {
        GUCEF_LOG( 0, "Setting Node Peer validators" );
        
        nodeA.SetPeerValidator( &testValidator );
        nodeB.SetPeerValidator( &testValidator );
        
        if ( nodeA.GetPeervalidator() != &testValidator )
        {
            GUCEF_ERROR_LOG( 0, "Setting Node Peer validators failed" );
            ERRORHERE;
        }
        
        if ( nodeB.GetPeervalidator() != &testValidator )
        {
            GUCEF_ERROR_LOG( 0, "Setting Node Peer validators failed" );
            ERRORHERE;
        }

        GUCEF_LOG( 0, "Attempting to listen on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
        if ( !nodeA.ListenOnPort( DRN_PEERCOMM_PORT ) )
        {
            // the listen actions should succeed for our tests to work
            GUCEF_ERROR_LOG( 0, "Failed to open listening socket on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
            ERRORHERE;
        }
        GUCEF_LOG( 0, "Successfully opened listen port" );
        
        GUCEF_LOG( 0, "Attempting to connect to node on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
        if ( !nodeB.ConnectToPeer( "127.0.0.1", DRN_PEERCOMM_PORT ) )
        {
            // since we are connecting localhost this should always work
            GUCEF_ERROR_LOG( 0, "Failed to connect to peer node on port " + GUCEF::CORE::Int32ToString( DRN_PEERCOMM_PORT ) );
            ERRORHERE;
        }
        GUCEF_LOG( 0, "Successfully connected to a listen port" );
        
        //if ( !nodeB.RequestDataGroupList() )
        {
        }
    }
      
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformPeerToPeerTest( void )
{GUCEF_TRACE;
  
    try
    {
        GUCEF_LOG( 0, "*** Commencing gucefDRN Peer to peer test ***" );
        
        CTestPeerToPeerSubSystem testSubSystem;
        CORE::CGUCEFApplication::Instance()->main( 0, NULL, true );
    }
    catch ( ... )
    {
        GUCEF_ERROR_LOG( 1, "unhandled exception during PerformPeerToPeerTest()" );
    } 
    
    GUCEF_LOG( 0, "*** Completed gucefDRN Peer to peer test ***" );
}

/*-------------------------------------------------------------------------*/
