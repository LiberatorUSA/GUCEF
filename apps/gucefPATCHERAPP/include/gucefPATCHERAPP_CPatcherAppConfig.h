/*
 *  gucefPATCHERAPP: Application module for the patcher application
 *  Copyright (C) 2002 - 2013.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_PATCHERAPP_CPATCHERAPPMAINCONFIG_H
#define GUCEF_PATCHERAPP_CPATCHERAPPMAINCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_PATCHERAPP_MACROS_H
#include "gucefPATCHERAPP_macros.h"
#define GUCEF_PATCHERAPP_MACROS_H
#endif /* GUCEF_PATCHERAPP_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHERAPP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CPatcherAppConfig : public CORE::CIConfigurable
{
    public:

    typedef std::vector< CString > TStringList;

    CPatcherAppConfig( void );

    virtual ~CPatcherAppConfig();
    
    const CString& GetWindowManagerName( void ) const;
   
    const CString& GetGuiBackendName( void ) const;

    const CString& GetInitialFormTypeName( void ) const;

    const CString& GetInitialFormResourcePath( void ) const;

    const CString& GetLogFilePath( void ) const;

    bool IsFileLoggerEnabled( void ) const;

    bool IsConsoleLoggerEnabled( void ) const;

    bool IsConsoleWindowEnabled( void ) const;

    const TStringList& GetFontAssetsToLoad( void ) const;

    void SetPatchEngineConfigFilePath( const CString& configFilePath );

    const CString& GetPatchEngineConfigFilePath( void ) const;

    void SetPatchEngineConfigFileCodec( const CString& configFileCodec );

    const CString& GetPatchEngineConfigFileCodec( void ) const;

    virtual bool SaveConfig( CORE::CDataNode& tree );

    virtual bool LoadConfig( const CORE::CDataNode& treeroot );

    void Clear( void );

    void InitToDefaultValues( void );

    private:

    CPatcherAppConfig( const CPatcherAppConfig& src );

    CPatcherAppConfig& operator=( const CPatcherAppConfig& src );
    
    private:

    CString m_windowManagerName;
    CString m_guiBackendName;
    CString m_initialFormTypeName;
    CString m_initialFormResourcePath;
    TStringList m_fontAssetsToLoad;
    CString m_logfilePath;
    bool m_isFileLoggerEnabled;
    bool m_isConsoleLoggerEnabled;
    bool m_isConsoleWindowEnabled;    
    CString m_patchEngineConfigFilePath;
    CString m_patchEngineConfigFileCodec;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHERAPP */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHERAPP_CPATCHERAPPMAINCONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-01-2013 :
        - Dinand: Initial version.

---------------------------------------------------------------------------*/