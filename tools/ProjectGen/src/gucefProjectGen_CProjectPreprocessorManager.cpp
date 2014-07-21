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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefProjectGen_CProjectPreprocessorManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PROJECTGEN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CProjectPreprocessorManager::CProjectPreprocessorManager( void )
    : m_projectPreprocessors()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CProjectPreprocessorManager::~CProjectPreprocessorManager()
{GUCEF_TRACE;

    m_projectPreprocessors.clear();
}

/*-------------------------------------------------------------------------*/
       
void
CProjectPreprocessorManager::RegisterProjectPreprocessor( CIProjectPreprocessor* preprocessor )
{GUCEF_TRACE;

    m_projectPreprocessors.insert( preprocessor );
}

/*-------------------------------------------------------------------------*/

void
CProjectPreprocessorManager::UnregisterProjectPreprocessor( CIProjectPreprocessor* preprocessor )
{GUCEF_TRACE;

    m_projectPreprocessors.erase( preprocessor );
}
    
/*-------------------------------------------------------------------------*/

const CProjectPreprocessorManager::TProjectPreprocessorsList&
CProjectPreprocessorManager::GetProjectPreprocessors( void ) const
{GUCEF_TRACE;

    return m_projectPreprocessors;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGEN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
