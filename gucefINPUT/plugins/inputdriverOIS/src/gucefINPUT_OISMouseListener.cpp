/*
 *  inputdriverOIS: GUCEF plugin module proving an input driver backend
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
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include "gucefINPUT_OISMouseListener.h"
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CMouseListener::CMouseListener( TInputCallbacks* callbacks ,
                                void* userData             )
    : OIS::MouseListener()     ,
      m_callbacks( callbacks ) ,
      m_userData( userData )
{

}

/*-------------------------------------------------------------------------*/
    
CMouseListener::~CMouseListener()
{

}

/*-------------------------------------------------------------------------*/

bool
CMouseListener::mouseMoved( const OIS::MouseEvent &arg )
{
    m_callbacks->onMouseMove( m_userData      ,
                              arg.state.X.abs ,
                              arg.state.Y.abs ,
                              arg.state.X.rel ,
                              arg.state.Y.rel );
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CMouseListener::mousePressed( const OIS::MouseEvent& arg , 
                              OIS::MouseButtonID id      )
{
    m_callbacks->onMouseButtonDown( m_userData ,
                                   (UInt32) id );
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CMouseListener::mouseReleased( const OIS::MouseEvent& arg , 
                               OIS::MouseButtonID id      )
{
    m_callbacks->onMouseButtonUp( m_userData ,
                                 (UInt32) id );
    return true;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
