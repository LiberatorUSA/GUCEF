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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CSYSCONSOLE_H
#include "CSysConsole.h"
#define GUCEF_CORE_CSYSCONSOLE_H
#endif /* GUCEF_CORE_CSYSCONSOLE_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CSysConsoleClient.h"

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

CSysConsoleClient::CSysConsoleClient( void )
        : _system( &CORE::CCoreGlobal::Instance()->GetSysConsole() )
{GUCEF_TRACE;

        _system->InitClient( this );
}

/*-------------------------------------------------------------------------*/

CSysConsoleClient::~CSysConsoleClient()
{GUCEF_TRACE;

        _system->UnregClient( this );
}

/*-------------------------------------------------------------------------*/

bool
CSysConsoleClient::RegisterCmd( const CString& path                ,
                                const CString& command             ,
                                const std::vector< CString >& args ,
                                CISysConsoleCmdHandler* cmdhandler )
{GUCEF_TRACE;

        return _system->RegisterCmd( path       ,
                                     command    ,
                                     args       ,
                                     cmdhandler );
}

/*-------------------------------------------------------------------------*/

void
CSysConsoleClient::UnregisterCmd( const CString& path    ,
                                  const CString& command )
{GUCEF_TRACE;

        _system->UnregisterCmd( path    ,
                                command );
}

/*-------------------------------------------------------------------------*/

void
CSysConsoleClient::LeaveDir( void )
{GUCEF_TRACE;

        _system->LeaveDir( this );
}

/*-------------------------------------------------------------------------*/

bool
CSysConsoleClient::EnterDir( const CString& dirname )
{GUCEF_TRACE;

        return _system->EnterDir( this, dirname );
}

/*-------------------------------------------------------------------------*/

bool
CSysConsoleClient::JumpTo( const CString& path )
{GUCEF_TRACE;

        return _system->JumpTo( this ,
                                path );
}

/*-------------------------------------------------------------------------*/

const CString&
CSysConsoleClient::GetPath( void ) const
{GUCEF_TRACE;

        return _path;
}

/*-------------------------------------------------------------------------*/

bool
CSysConsoleClient::Execute( const CString& functionname            ,
                            const std::vector< CString >& arglist  ,
                            std::vector< CString >& resultdata     )
{GUCEF_TRACE;

        return _system->Execute( this         ,
                                 functionname ,
                                 arglist      ,
                                 resultdata   );
}

/*-------------------------------------------------------------------------*/

std::vector< CString >
CSysConsoleClient::GetDirList( void ) const
{GUCEF_TRACE;

        return _system->GetDirList( this );
}

/*-------------------------------------------------------------------------*/

std::vector< CString >
CSysConsoleClient::GetCmdList( void ) const
{GUCEF_TRACE;

        return _system->GetCmdList( this );
}

/*-------------------------------------------------------------------------*/

bool
CSysConsoleClient::ProcessUserInput( const CString& userInput  ,
                                     TStringVector& resultdata )
{GUCEF_TRACE;

    Int32 subStrIndex = userInput.HasSubstr( "cd", true );
    if ( subStrIndex == 0 )
    {
         CString dirString = userInput.CutChars( 3, true );
         if ( dirString.Length() > 0 )
         {
            if ( 0 == dirString.HasSubstr( "..", true ) )
            {
                LeaveDir();
                return true;
            }
            dirString = dirString.ReplaceChar( '/', '\\' );
            return JumpTo( dirString );
         }
    }
    else
    {
        subStrIndex = userInput.HasSubstr( "dirs", true );
        if ( subStrIndex == 0 )
        {
            resultdata = GetDirList();
            return true;
        }
        else
        {
            subStrIndex = userInput.HasSubstr( "cmds", true );
            if ( subStrIndex == 0 )
            {
                resultdata = GetCmdList();
                return true;
            }
        }
    }

    TStringVector elements = userInput.ParseElements( ' ', false );
    if ( elements.size() > 0 )
    {
        CString funcName = *elements.begin();
        elements.erase( elements.begin() );
        return Execute( funcName   ,
                        elements   ,
                        resultdata );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CSysConsoleClient::OnReturnData( const TStringVector& data )
{GUCEF_TRACE;

    /* dummy, to avoid mandatory implementation for descending classes */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

