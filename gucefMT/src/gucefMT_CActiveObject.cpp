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

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"         /* OS wrappers for threading functionality */
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */ 

#include "gucefMT_CActiveObject.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CActiveObject::CActiveObject( void )
        : _taskdata( NULL ) ,
          _delay( 10 )      ,
          _active( false )
{
         
}

/*-------------------------------------------------------------------------*/

CActiveObject::CActiveObject( const CActiveObject& src )
        : _taskdata( src._taskdata ) ,
          _delay( src._delay )       ,
          _active( false )
{
        if ( src.IsActive() )
        {
                Activate();
        }        
}

/*-------------------------------------------------------------------------*/
        
CActiveObject::CActiveObject( void* taskdata ,
                              UInt32 delay   )
        : _taskdata( taskdata ) ,
          _delay( delay )       ,
          _active( false )
{
        
}

/*-------------------------------------------------------------------------*/
        
CActiveObject::~CActiveObject()
{
        Deactivate( true );        
}

/*-------------------------------------------------------------------------*/
        
bool 
CActiveObject::IsActive( void ) const
{
        return _active; 
}

/*-------------------------------------------------------------------------*/

void 
CActiveObject::Activate( void )
{
        Activate( _taskdata );        
}

/*-------------------------------------------------------------------------*/

UInt32 
CActiveObject::OnActivate( void* thisobject )
{
        CActiveObject* tao = (CActiveObject*) thisobject;
        void* taskdata = tao->GetTaskData();
        
        tao->OnTaskStart( taskdata );
        
        bool taskfinished = false;
        while ( !taskfinished && tao->_active )
        {
                if ( tao->_suspend )
                {
                        ThreadSuspend( tao->_td );
                }
                taskfinished = tao->OnTaskCycle( taskdata );
                
                if ( !taskfinished )
                {
                        ThreadDelay( tao->_delay );
                }
        }
        
        tao->OnTaskEnd( taskdata );
        
        tao->_td = NULL;
        return 1;
}

/*-------------------------------------------------------------------------*/

void 
CActiveObject::Activate( void* taskdata )
{
        if ( _active ) return;
        
        _taskdata = taskdata;
        _td = ThreadCreate( OnActivate ,
                            this       );
        _active = true;                                            
}

/*-------------------------------------------------------------------------*/
        
void 
CActiveObject::Deactivate( bool force )
{        
        if ( force )
        {
                /*
                 *      Kill the thread
                 */
                ThreadKill( _td );
        }
        _active = false;
}

/*-------------------------------------------------------------------------*/

void 
CActiveObject::Pause( void )
{
        _suspend = true;
        _active = false;
}

/*-------------------------------------------------------------------------*/

void 
CActiveObject::Resume( void )
{
        if ( _td && _suspend )
        {
                ThreadResume( _td );
                _active = true;
        }                
}

/*-------------------------------------------------------------------------*/
        
void* 
CActiveObject::GetTaskData( void ) const
{
        return _taskdata;
}

/*-------------------------------------------------------------------------*/
        
CActiveObject& 
CActiveObject::operator=( const CActiveObject& src )
{
        Deactivate( true );
        
        _taskdata = src._taskdata;
        _delay = src._delay;
        _suspend = false;
        
        if ( src.IsActive() )
        {
                Activate();
        }       
        
        return *this;  
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
