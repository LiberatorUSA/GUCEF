/*
 *  ProjectGenerator: Tool to generate module/project files
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_PROJECTGEN_CDIRPREPROCESSORMANAGER_H
#include "gucefProjectGen_CDirPreprocessorManager.h"
#define GUCEF_PROJECTGEN_CDIRPREPROCESSORMANAGER_H
#endif /* GUCEF_PROJECTGEN_CDIRPREPROCESSORMANAGER_H ? */

#include "gucefProjectGen_CProjectGenGlobal.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PROJECTGEN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CProjectGenGlobal* CProjectGenGlobal::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CProjectGenGlobal*
CProjectGenGlobal::Instance()
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_instance = new CProjectGenGlobal();
        g_instance->Initialize();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CProjectGenGlobal::Deinstance( void )
{GUCEF_TRACE;

    delete g_instance;
    g_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void
CProjectGenGlobal::Initialize( void )
{GUCEF_TRACE;

    CORE::CCoreGlobal::Instance();
    
    /*
     *  Instantiate all the singletons
     */
    m_dirPreprocessorManager = new CDirPreprocessorManager();

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefProjectGen Global systems initialized" );
}

/*-------------------------------------------------------------------------*/

CProjectGenGlobal::CProjectGenGlobal( void )
    : m_dirPreprocessorManager( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CProjectGenGlobal::~CProjectGenGlobal()
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Shutting down gucefProjectGen global systems" );

    /*
     *      cleanup all singletons
     */
    delete m_dirPreprocessorManager;
    m_dirPreprocessorManager = NULL;
}

/*-------------------------------------------------------------------------*/

CDirPreprocessorManager&
CProjectGenGlobal::GetDirPreprocessorManager( void )
{GUCEF_TRACE;

    return *m_dirPreprocessorManager;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGEN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
