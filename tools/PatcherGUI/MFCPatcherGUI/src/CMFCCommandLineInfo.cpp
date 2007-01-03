/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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
{TRACE;

}

/*-------------------------------------------------------------------------*/

CMFCCommandLineInfo::~CMFCCommandLineInfo()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CMFCCommandLineInfo::ParseParam( const char* pszParam , 
                                 BOOL bFlag           , 
                                 BOOL bLast           )
{TRACE;

    m_paramList.push_back( pszParam );
    m_singleString += pszParam;
}

/*-------------------------------------------------------------------------*/

const CMFCCommandLineInfo::TParamList&
CMFCCommandLineInfo::GetParamList( void ) const
{TRACE;

    return m_paramList;
}

/*-------------------------------------------------------------------------*/

const GUCEF::CORE::CString&
CMFCCommandLineInfo::GetParamString( void ) const
{TRACE;

    return m_singleString;
}

/*-------------------------------------------------------------------------*/
