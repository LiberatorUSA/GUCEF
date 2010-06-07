/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_CSTDCODECPLUGIN_H
#define GUCEF_CORE_CSTDCODECPLUGIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CICODECPLUGIN_H
#include "CICodecPlugin.h"
#define GUCEF_CORE_CICODECPLUGIN_H
#endif /* GUCEF_CORE_CICODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  GUCEF CORE module plugin implementation for codecs
 */
class GUCEFCORE_EXPORT_CPP CStdCodecPlugin : public CICodecPlugin
{
    public:

    CStdCodecPlugin( void );

    virtual ~CStdCodecPlugin();

    virtual bool GetCodecs( CCodecSet& codecSet );

    virtual bool GetCodecList( CCodecList& codecList );

    virtual bool Encode( CIOAccess& source         ,
                         CIOAccess& dest           ,
                         const CString& familyName ,
                         const CString& typeName   );

    virtual bool Decode( CIOAccess& source         ,
                         CIOAccess& dest           ,
                         const CString& familyName ,
                         const CString& typeName   );

    virtual CString GetDescription( void ) const;

    virtual CString GetCopyright( void ) const;

    virtual TVersion GetVersion( void ) const;

    virtual CString GetModulePath( void ) const;

    virtual bool IsLoaded( void ) const;

    virtual bool Load( const CString& pluginPath );

    virtual bool Unload( void );

    private:

    CStdCodecPlugin( const CStdCodecPlugin& src );            /**< not implemented */
    CStdCodecPlugin& operator=( const CStdCodecPlugin& src ); /**< not implemented */

    void LinkCodecSet( void );
    void UnlinkCodecSet( void );

    private:

    CCodecSet m_codecSet;
    CCodecList m_codecList;
    CString m_modulePath;
    TDefaultFuncPtr m_fpTable[ 10 ];  /**< function pointer table */
    void* m_soHandle;                 /**< access to the so module */
    void* m_pluginData;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace CORE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTDCODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 15-12-2006 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
