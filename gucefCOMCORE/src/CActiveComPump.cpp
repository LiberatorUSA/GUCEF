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

#ifndef CSOCKET_H
#include "CSocket.h"
#define CSOCKET_H
#endif /* CSOCKET_H ? */

#ifndef CCOM_H
#include "CCom.h"
#define CCOM_H
#endif /* CCOM_H ? */

#include "CActiveComPump.h"

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CActiveComPump::CActiveComPump( void )
        : CActiveObject()                         ,
          _sockets( &CCom::Instance()->_sockets ) ,
          _datalock( &CCom::Instance()->_mutex )
{
}

/*-------------------------------------------------------------------------*/

CActiveComPump::CActiveComPump( const CActiveComPump& src )
{
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CActiveComPump::~CActiveComPump()
{
}

/*-------------------------------------------------------------------------*/

CActiveComPump&
CActiveComPump::operator=( const CActiveComPump& src )
{
        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/                      

bool 
CActiveComPump::OnTaskStart( void* taskdata )
{
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CActiveComPump::OnTaskCycle( void* taskdata )
{
        _datalock->Lock();        
        for ( Int32 i=0; i<=_sockets->GetLast(); ++i )
        {
                if ( _sockets->operator[]( i ) )
                {
                        ( (CSocket*)((*_sockets)[ i ]) )->Update();
                }
        }
        _datalock->Unlock();
        return true;
}
 
/*-------------------------------------------------------------------------*/ 
        
void 
CActiveComPump::OnTaskEnd( void* taskdata )
{
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/