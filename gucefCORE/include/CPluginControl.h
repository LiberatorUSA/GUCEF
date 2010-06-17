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

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#define GUCEF_CORE_CPLUGINCONTROL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_GUCEFMT_H
#include "gucefMT.h"                   /* MT library API */
#define GUCEF_MT_GUCEFMT_H
#endif /* GUCEF_MT_GUCEFMT_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"                /* platform string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"            /* platform dynamic array implementation */
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"         /* often used gucef macros */
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
 *      Forward declarations of framework classes used here
 */
class CPluginManager;
class CString;

/*-------------------------------------------------------------------------*/

/**
 *      Global framework plugin control center
 *      All plugin managers derived from CPluginManager automaticly register here.
 *
 */
class GUCEF_CORE_PUBLIC_CPP CPluginControl
{
        public:

        static CPluginControl* Instance( void );

        void LoadAll( void );

        void UnloadAll( void );
        
        bool Load( const CString& pluginPath );

        void SetPluginDir( const CString& path );

        CString GetPluginDir( void ) const;

        private:
        friend class CPluginManager;

        void Register( CPluginManager* pman );

        void Unregister( CPluginManager* pman );

        private:
        friend class CGUCEFCOREModule;

        static void Deinstance( void );

        private:

        CPluginControl( void );
        CPluginControl( const CPluginControl& src );
        ~CPluginControl();
        CPluginControl& operator=( const CPluginControl& src );

        static MT::CMutex _mutex;
        static CPluginControl* _instance;

        CString _plugindir;
        CDynamicArray _managers;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Initial implementation

-----------------------------------------------------------------------------*/
