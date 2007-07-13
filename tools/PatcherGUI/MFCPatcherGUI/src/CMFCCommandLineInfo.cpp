/*
 *  MFCPatcherGUI: GUI shell for the gucefPATCHER module
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CMFCCommandLineInfo.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CMFCCommandLineInfo::CMFCCommandLineInfo( void )
    : CCommandLineInfo() ,
      m_paramList()      ,
      m_singleString()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMFCCommandLineInfo::~CMFCCommandLineInfo()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CMFCCommandLineInfo::ParseParam( const char* pszParam , 
                                 BOOL bFlag           , 
                                 BOOL bLast           )
{GUCEF_TRACE;

    m_paramList.push_back( pszParam );
    m_singleString += pszParam;
}

/*-------------------------------------------------------------------------*/

const CMFCCommandLineInfo::TParamList&
CMFCCommandLineInfo::GetParamList( void ) const
{GUCEF_TRACE;

    return m_paramList;
}

/*-------------------------------------------------------------------------*/

const GUCEF::CORE::CString&
CMFCCommandLineInfo::GetParamString( void ) const
{GUCEF_TRACE;

    return m_singleString;
}

/*-------------------------------------------------------------------------*/
