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

#include "CHTTPClientInterface.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPClientInterface::CHTTPClientInterface( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CHTTPClientInterface::CHTTPClientInterface( const CHTTPClientInterface& src )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CHTTPClientInterface::~CHTTPClientInterface()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CHTTPClientInterface&
CHTTPClientInterface::operator=( const CHTTPClientInterface& src )
{TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClientInterface::OnConnected( CHTTPClient& client )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
void 
CHTTPClientInterface::OnDisconnected( CHTTPClient& client )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
void 
CHTTPClientInterface::OnHTTPError( CHTTPClient& client ,
                                   THTTPCODE errorcode )
{TRACE;
}

/*-------------------------------------------------------------------------*/                                   
        
void 
CHTTPClientInterface::OnError( CHTTPClient& client )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
void 
CHTTPClientInterface::OnRedirect( CHTTPClient& client           ,
                                  const CORE::CString& location )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
void 
CHTTPClientInterface::OnDataRecieved( CHTTPClient& client ,
                                      const char *data    ,
                                      UInt32 length       )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClientInterface::OnDataSend( CHTTPClient &client ,
                                  const char* data    ,
                                  UInt32 length       )
{TRACE;
}                                  

/*-------------------------------------------------------------------------*/

void 
CHTTPClientInterface::OnConnecting( CHTTPClient& client )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClientInterface::OnFinish( CHTTPClient &client )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CHTTPClientInterface::OnContent( CHTTPClient& client      ,
                                 const UInt32 size        ,
                                 const bool resumeable    ,
                                 const THTTPCODE httpcode )
{TRACE;
}                                 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
                                  