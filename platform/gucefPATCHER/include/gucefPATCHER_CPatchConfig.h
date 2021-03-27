/*
 *  gucefPATCHER: GUCEF RAD module providing a patch delivery system
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

#ifndef GUCEF_PATCHER_CPATCHCONFIG_H
#define GUCEF_PATCHER_CPATCHCONFIG_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_PATCHER_MACROS_H
#include "gucefPATCHER_macros.h"
#define GUCEF_PATCHER_MACROS_H
#endif /* GUCEF_PATCHER_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/ 

/**
 *  Strongly typed configuration storage
 */
class GUCEF_PATCHER_EXPORT_CPP CPatchConfig : public CORE::CIConfigurable
{
    public:
    
    CPatchConfig( void );
    
    CPatchConfig( const CPatchConfig& src );
    
    virtual ~CPatchConfig();
    
    CPatchConfig& operator=( const CPatchConfig& src );
    
    virtual bool SaveConfig( CORE::CDataNode& tree ) const;
                                   
    virtual bool LoadConfig( const CORE::CDataNode& treeroot );
    
    bool AddEngineStartTriggerEvent( const CORE::CEvent& triggerEvent );   
    
    bool RemoveEngineStartTriggerEvent( const CORE::CEvent& triggerEvent );
    
    bool HasEngineStartTriggerEvent( const CORE::CEvent& triggerEvent );
    
    bool AddEngineStopTriggerEvent( const CORE::CEvent& triggerEvent );   
    
    bool RemoveEngineStopTriggerEvent( const CORE::CEvent& triggerEvent );    
    
    bool HasEngineStopTriggerEvent( const CORE::CEvent& triggerEvent );

    bool SetLocalRootDir( const CORE::CString& localRoot );
    
    const CORE::CString& GetLocalRootDir( void ) const;
    
    bool SetLocalTempStorageRootDir( const CORE::CString& tempStorageRoot );
    
    const CORE::CString& GetLocalTempStorageRootDir( void ) const;
    
    bool SetPatchListURL( const CORE::CString& patchListURL );
    
    const CORE::CString& GetPatchListURL( void ) const;

    bool SetPatchListCodec( const CORE::CString& patchListCodec );
    
    const CORE::CString& GetPatchListCodec( void ) const;
    
    void SetStopOnFileReplacementFailure( const bool stopOnFailure );
    
    bool GetStopOnFileReplacementFailure( void ) const;

    virtual const CString& GetClassTypeName( void ) const;
    
    private:
    
    CORE::CString m_patchListURL;
    CORE::CString m_patchListCodec;
    CORE::CString m_localRoot;
    CORE::CString m_tempStorageRoot;
    std::set< CORE::CEvent > m_startTriggers;
    std::set< CORE::CEvent > m_stopTriggers;
    bool m_stopOnReplacementFailure;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHCONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-12-2006 :
        - Dinand: Initial version

----------------------------------------------------------------------------*/