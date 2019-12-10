/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "CInputContext.h"

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

CInputContext::CInputContext( CInputDriver& inputDriver      ,
                              const CORE::CValueList& params )
        : m_valuelist( params )         ,
          m_id()                        ,
          m_inputDriver( &inputDriver )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CInputContext::~CInputContext()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
CInputContext::GetContextParams( void ) const
{GUCEF_TRACE;
    
    return m_valuelist;
}

/*-------------------------------------------------------------------------*/

void 
CInputContext::SetID( TContextId& id )
{GUCEF_TRACE;
    
    m_id = id;
}

/*-------------------------------------------------------------------------*/

Int32 
CInputContext::GetID( void ) const
{GUCEF_TRACE;
    
    return m_id;
}

/*-------------------------------------------------------------------------*/

CInputDriver*
CInputContext::GetDriver( void )
{GUCEF_TRACE;

    return m_inputDriver;
}
       
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
