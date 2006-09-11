/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
                                const CStringList& args            ,
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
CSysConsoleClient::Execute( const CString& functionname ,
                            const CStringList& arglist  ,
                            CStringList& resultdata     )
{
        return _system->Execute( this         , 
                                 functionname ,
                                 arglist      ,
                                 resultdata   );     
}

/*-------------------------------------------------------------------------*/                      

CStringList 
CSysConsoleClient::GetDirList( void ) const
{
        return _system->GetDirList( this );
}

/*-------------------------------------------------------------------------*/

CStringList 
CSysConsoleClient::GetCmdList( void ) const
{
        return _system->GetCmdList( this );
}

/*-------------------------------------------------------------------------*/

void 
CSysConsoleClient::OnReturnData( const CStringList& data )
{
        /* dummy, to avoid manditory implementation for decending classes */        
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

