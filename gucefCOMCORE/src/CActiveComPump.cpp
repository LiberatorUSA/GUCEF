/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
        : _tickcount( 0 )                         ,
          _deltaticks( 0 )                        ,
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

void
CActiveComPump::Update( UInt32 tickcount  ,
                        UInt32 deltaticks )
{
        _datalock->Lock();
        _tickcount = tickcount;
        _deltaticks = deltaticks;
        _datalock->Unlock();
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
                        ( (CSocket*)((*_sockets)[ i ]) )->Update( _tickcount  ,
                                                                  _deltaticks );
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