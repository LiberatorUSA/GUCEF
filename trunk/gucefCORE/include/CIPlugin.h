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

#ifndef GUCEF_CORE_CIPLUGIN_H
#define GUCEF_CORE_CIPLUGIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#include "ExceptionMacros.h"
#define GUCEF_CORE_EXCEPTIONMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"         /* often used gucef data structs */
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */

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
 *  Interface class for plugins
 */
class GUCEF_CORE_PUBLIC_CPP CIPlugin
{
    public:

    CIPlugin( void );

    CIPlugin( const CIPlugin& src );

    virtual ~CIPlugin();

    CIPlugin& operator=( const CIPlugin& src );

    virtual CString GetDescription( void ) const = 0;

    virtual CString GetCopyright( void ) const = 0;

    virtual TVersion GetVersion( void ) const = 0;

    virtual CString GetModulePath( void ) const = 0;

    virtual bool IsLoaded( void ) const = 0;

    virtual bool Load( const CString& pluginPath ) = 0;

    virtual bool Unload( void ) = 0;

    GUCEF_DEFINE_MSGEXCEPTION( GUCEF_CORE_PUBLIC_CPP, ENotLoaded );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
