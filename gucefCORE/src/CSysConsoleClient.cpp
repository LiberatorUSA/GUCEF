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

#ifndef GUCEF_CORE_CSYSCONSOLE_H
#include "CSysConsole.h"
#define GUCEF_CORE_CSYSCONSOLE_H 
#endif /* GUCEF_CORE_CSYSCONSOLE_H ? */

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
        : _system( CSysConsole::Instance() )
{
        _system->InitClient( this );        
}

/*-------------------------------------------------------------------------*/

CSysConsoleClient::CSysConsoleClient( const CSysConsoleClient& src )
{
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CSysConsoleClient::~CSysConsoleClient()
{
        _system->UnregClient( this );
}

/*-------------------------------------------------------------------------*/

CSysConsoleClient&
CSysConsoleClient::operator=( const CSysConsoleClient& src )
{
        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsoleClient::RegisterCmd( const CString& path                ,
                                const CString& command             ,
                                const std::vector< CString >& args ,
                                CISysConsoleCmdHandler* cmdhandler )
{                                          
        return _system->RegisterCmd( path       ,
                                     command    ,
                                     args       ,
                                     cmdhandler );
}

/*-------------------------------------------------------------------------*/                          

void 
CSysConsoleClient::UnregisterCmd( const CString& path    ,
                                  const CString& command )
{
        _system->UnregisterCmd( path    ,
                                command );
}

/*-------------------------------------------------------------------------*/                            

void 
CSysConsoleClient::LeaveDir( void )
{
        _system->LeaveDir( this );
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsoleClient::EnterDir( const CString& dirname )
{
        return _system->EnterDir( this, dirname );
}

/*-------------------------------------------------------------------------*/                       

bool 
CSysConsoleClient::JumpTo( const CString& path )
{
        return _system->JumpTo( this , 
                                path );
}

/*-------------------------------------------------------------------------*/                     

const CString& 
CSysConsoleClient::GetPath( void ) const
{
        return _path;
}

/*-------------------------------------------------------------------------*/

bool 
CSysConsoleClient::Execute( const CString& functionname            ,
                            const std::vector< CString >& arglist  ,
                            std::vector< CString >& resultdata     )
{
        return _system->Execute( this         , 
                                 functionname ,
                                 arglist      ,
                                 resultdata   );     
}

/*-------------------------------------------------------------------------*/                      

std::vector< CString > 
CSysConsoleClient::GetDirList( void ) const
{
        return _system->GetDirList( this );
}

/*-------------------------------------------------------------------------*/

std::vector< CString > 
CSysConsoleClient::GetCmdList( void ) const
{
        return _system->GetCmdList( this );
}

/*-------------------------------------------------------------------------*/

void 
CSysConsoleClient::OnReturnData( const std::vector< CString >& data )
{
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

