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

#ifndef GUCEF_CORE_CONFIGSTORE_H
#define GUCEF_CORE_CONFIGSTORE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"           /* gucefMT mutex class */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"                /* framework string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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

/*
 *      Forward declaration of the abstract base for configurable items.
 */
class CIConfigurable;

/*-------------------------------------------------------------------------*/

/**
 *      Centralized configuration control.
 *      Allows you load and save the config of all configureable items that
 *      have global config usage enabled.
 */
class GUCEF_CORE_PUBLIC_CPP CConfigStore
{
    public:

    static CConfigStore* Instance( void );

    void SetConfigFile( const CString& filepath );

    CString GetConfigFile( void ) const;

    bool SaveConfig( const CString& name  ,
                     bool preserve = true );

    bool LoadConfig( void );

    void SetCodec( const CString& codectype );

    CString GetCodec( void ) const;

    private:

    friend class CIConfigurable;

    void Register( CIConfigurable* configobj );

    void Unregister( CIConfigurable* configobj );

    private:
    friend class CGUCEFCOREModule;

    static void Deinstance( void );

    private:

    CConfigStore( void );
    CConfigStore( const CConfigStore& src );
    CConfigStore& operator=( const CConfigStore& src );
    ~CConfigStore();

    private:

    typedef std::set< CIConfigurable* > TConfigurableSet;

    static CConfigStore* _instance;

    CString _codectype;
    CString _configfile;
    TConfigurableSet _configobjs;
    MT::CMutex _datalock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
