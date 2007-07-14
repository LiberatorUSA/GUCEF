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

#ifndef MFCWIN32_CMFCCOMMANDLINEINFO_H
#define MFCWIN32_CMFCCOMMANDLINEINFO_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>
#include <afxwin.h> /* needed for CCommandLineInfo */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CMFCCommandLineInfo : public CCommandLineInfo
{
    public:
    
    typedef std::vector< GUCEF::CORE::CString > TParamList;
    
    CMFCCommandLineInfo( void );
    virtual ~CMFCCommandLineInfo();
    
    virtual void ParseParam( const char* pszParam , 
                             BOOL bFlag           , 
                             BOOL bLast           );
    
    const TParamList& GetParamList( void ) const;
    
    const GUCEF::CORE::CString& GetParamString( void ) const;
    
    private:
    
    CMFCCommandLineInfo( const CMFCCommandLineInfo& src );
    CMFCCommandLineInfo& operator=( const CMFCCommandLineInfo& src ); 
    
    private:
    
    TParamList m_paramList;
    GUCEF::CORE::CString m_singleString;
};

/*-------------------------------------------------------------------------*/

#endif /* MFCWIN32_CMFCCOMMANDLINEINFO_H ? */