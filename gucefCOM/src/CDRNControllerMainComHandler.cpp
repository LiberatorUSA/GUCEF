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

#ifndef CDRNCONTROLLER_H
#include "CDRNController.h"
#define CDRNCONTROLLER_H
#endif /* CDRNCONTROLLER_H ? */

#include "CDRNControllerMainComHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

CDRNControllerMainComHandler::CDRNControllerMainComHandler( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNControllerMainComHandler::CDRNControllerMainComHandler( const CDRNControllerMainComHandler& src )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNControllerMainComHandler::CDRNControllerMainComHandler( CDRNController* controller )
        : m_controller( controller )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNControllerMainComHandler&
CDRNControllerMainComHandler::operator=( const CDRNControllerMainComHandler& src )
{TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

CDRNControllerMainComHandler::~CDRNControllerMainComHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CDRNControllerMainComHandler::OnClientRead( COM::CBPTCPServerSocket &socket     ,
                                            COM::CBPTCPServerConnection& client ,
                                            UInt32 connection                     ,
                                            const char *data                      ,
                                            UInt32 size                           ,
                                            UInt32 &keepbytes                     )
{TRACE;
        m_controller->OnMainComClientRead( socket     ,
                                           client     ,
                                           connection ,
                                           data       ,
                                           size       ,
                                           keepbytes  );
}

/*-------------------------------------------------------------------------*/
                                            
void 
CDRNControllerMainComHandler::OnClientWrite( COM::CBPTCPServerSocket &socket     ,
                                             COM::CBPTCPServerConnection& client ,
                                             UInt32 connection                   ,
                                             const char *data                    ,
                                             UInt32 size                         )
{TRACE;
        m_controller->OnMainComClientWrite( socket     ,
                                            client     ,
                                            connection ,
                                            data       ,
                                            size       );
}                                             

/*-------------------------------------------------------------------------*/

void 
CDRNControllerMainComHandler::OnClientConnected( COM::CBPTCPServerSocket &socket        ,
                                                 COM::CBPTCPServerConnection& clientcon ,
                                                 UInt32 connection                      )
{TRACE;
        m_controller->OnMainComClientConnected( socket     ,                                                
                                                clientcon  ,
                                                connection );
}

/*-------------------------------------------------------------------------*/                                                 

void 
CDRNControllerMainComHandler::OnClientDisconnect( COM::CBPTCPServerSocket &socket     ,
                                                  COM::CBPTCPServerConnection& client , 
                                                  UInt32 connection                   ,
                                                  bool clientclose                    ) 
{TRACE;
        m_controller->OnMainComClientDisconnect( socket      ,
                                                 client      , 
                                                 connection  ,
                                                 clientclose );
}

/*-------------------------------------------------------------------------*/
                                                  
void 
CDRNControllerMainComHandler::OnClientError( COM::CBPTCPServerSocket &socket     ,
                                             COM::CBPTCPServerConnection& client ,
                                             UInt32 connection                   ,
                                             TSocketError *errorcode             )
{TRACE;
        m_controller->OnMainComClientError( socket     ,
                                            client     ,
                                            connection ,
                                            errorcode  );
}

/*-------------------------------------------------------------------------*/                                             

void 
CDRNControllerMainComHandler::OnOpen( COM::CBPTCPServerSocket &socket )
{TRACE;
        m_controller->OnMainComOpen( socket );
}

/*-------------------------------------------------------------------------*/

void 
CDRNControllerMainComHandler::OnClose( COM::CBPTCPServerSocket &socket )
{TRACE;
        m_controller->OnMainComClose( socket );
}

/*-------------------------------------------------------------------------*/

void 
CDRNControllerMainComHandler::OnMaxConnectionsChanged( COM::CBPTCPServerSocket &socket ,
                                                       UInt32 old_max                  ,
                                                       UInt32 new_max                  )
{TRACE;
        m_controller->OnMainComMaxConnectionsChanged( socket  ,
                                                      old_max ,
                                                      new_max );
}

/*-------------------------------------------------------------------------*/                                                       

void 
CDRNControllerMainComHandler::OnError( COM::CBPTCPServerSocket &socket ,
                                       TSocketError errorcode          )
{TRACE;
        m_controller->OnMainComError( socket    ,
                                      errorcode );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
                                       