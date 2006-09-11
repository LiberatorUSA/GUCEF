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

#include "CStateTransition.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
CORE_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CStateTransition::CStateTransition( UInt32 oldstateid              ,
                                    CStateHandler* oldstatehandler ,
                                    UInt32 newstateid              ,
                                    CStateHandler* newstatehandler ,
                                    UInt32 conditionvalue          )
        : _oldstateid( oldstateid )           ,
          _oldstatehandler( oldstatehandler ) ,
          _newstateid( newstateid )           ,
          _newstatehandler( newstatehandler ) ,
          _conditionvalue( conditionvalue )                       
{

}

/*-------------------------------------------------------------------------*/

CStateTransition::CStateTransition( void )
{

}

/*-------------------------------------------------------------------------*/
                          
CStateTransition::CStateTransition( const CStateTransition& src )
        : _oldstateid( _oldstateid )           ,
          _oldstatehandler( _oldstatehandler ) ,
          _newstateid( _newstateid )           ,
          _newstatehandler( _newstatehandler ) ,
          _conditionvalue( _conditionvalue )
{
        
}

/*-------------------------------------------------------------------------*/

CStateTransition& 
CStateTransition::operator=( const CStateTransition& src )
{
        if ( this != &src )
        {
                _oldstateid = _oldstateid;
                _oldstatehandler = _oldstatehandler;
                _newstateid  = _newstateid;
                _newstatehandler = _newstatehandler;
                _conditionvalue = _conditionvalue;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CStateTransition::GetOldStateID( void ) const
{
        return _oldstateid;        
}

/*-------------------------------------------------------------------------*/
        
CStateHandler* 
CStateTransition::GetOldStateHandler( void ) const
{
        return _oldstatehandler;
}

/*-------------------------------------------------------------------------*/
        
CStateHandler* 
CStateTransition::GetNewStateHandler( void ) const
{
        return _newstatehandler;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CStateTransition::GetNewStateID( void ) const
{
        return _newstateid;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CStateTransition::GetConditionValue( void ) const
{
        return _conditionvalue;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CORE_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
