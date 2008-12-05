/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#include "gucefCOMCORE_icmpApi.h"

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

/* #include <Icmpapi.h> -> this is the header for the functions that we dynamicly link */  
#define WIN32_LEAN_AND_MEAN
#include <winsock.h>                  /* windows networking API, used here for it's type declarations */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex globalMutex;
UInt32 DllRefCount = 0;
void* DllHandle = NULL;
TIcmpCreateFilePtr IcmpCreateFile = NULL;
TIcmpCloseHandlePtr IcmpCloseHandle = NULL;
TIcmpSendEchoPtr IcmpSendEcho = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
LinkICMPModule( const char* moduleName )
{
    globalMutex.Lock();
    if ( DllHandle == NULL )
    {
        DllHandle = CORE::LoadModuleDynamicly( moduleName );
        if ( DllHandle != NULL )
        {
            IcmpCreateFile = (TIcmpCreateFilePtr) CORE::GetFunctionAddress( DllHandle         ,
                                                                            "IcmpCreateFile"  ,
                                                                            0                 );
            IcmpCloseHandle = (TIcmpCloseHandlePtr) CORE::GetFunctionAddress( DllHandle         ,
                                                                              "IcmpCloseHandle" ,
                                                                              sizeof( HANDLE )  );
            IcmpSendEcho = (TIcmpSendEchoPtr) CORE::GetFunctionAddress( DllHandle         ,
                                                                        "IcmpSendEcho"    ,
                                                                        sizeof( HANDLE ) + sizeof( IPAddr ) + 2*sizeof( LPVOID ) + sizeof( WORD ) + sizeof( PIP_OPTION_INFORMATION ) + 2*sizeof( DWORD ) );
                                                     
            bool foundFunctions = ( IcmpCreateFile != NULL ) && ( IcmpCloseHandle != NULL ) && ( IcmpSendEcho != NULL );
            if ( !foundFunctions )
            {
                CORE::UnloadModuleDynamicly( DllHandle );
                DllHandle = NULL;
                DllRefCount = 0;
                
                globalMutex.Unlock();
                return false;            
            }
        
            // Successfully linked the module and functions
        }
        else
        {
            DllHandle = NULL;
            DllRefCount = 0;
            
            globalMutex.Unlock();
            return false;            
        }
    }
    
    ++DllRefCount;
    
    globalMutex.Unlock();
    return true;
}

/*-------------------------------------------------------------------------*/

bool
LinkICMPOlderThenXP( void )
{
    return LinkICMPModule( "ICMP.dll" );
}

/*-------------------------------------------------------------------------*/

bool
LinkICMPForXPAndHigher( void )
{
    return LinkICMPModule( "iphlpapi.dll" );
}

/*-------------------------------------------------------------------------*/

void
UnlinkICMP( void )
{
    globalMutex.Lock();    
    if ( DllHandle != NULL )
    {
        if ( DllRefCount == 1 )
        {
            CORE::UnloadModuleDynamicly( DllHandle );
            DllHandle = NULL;            
            --DllRefCount;
        }
    }
    globalMutex.Unlock();
}

/*-------------------------------------------------------------------------*/

Int32
LinkICMP( void )
{GUCEF_CORE;
    
    if ( !LinkICMPForXPAndHigher() )
    {
        if ( LinkICMPOlderThenXP() )
        {
            return 1;
        }
    }
    return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COMCORE ? */
} /* namespace GUCEF ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */
