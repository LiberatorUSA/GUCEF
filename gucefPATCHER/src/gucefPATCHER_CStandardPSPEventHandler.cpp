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

#include "dvfileutils.h"

#include "gucefPATCHER_CStandardPSPEventHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/
 
CStandardPSPEventHandler::CStandardPSPEventHandler( void )
        : CPatchSetParserEventHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CStandardPSPEventHandler::CStandardPSPEventHandler( const CStandardPSPEventHandler& src )
        : CPatchSetParserEventHandler( src )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CStandardPSPEventHandler::~CStandardPSPEventHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CStandardPSPEventHandler&
CStandardPSPEventHandler::operator=( const CStandardPSPEventHandler& src )
{TRACE;
        CPatchSetParserEventHandler::operator=( src );
        
        if ( &src != this )
        {
                
        }
        return *this; 
}

/*-------------------------------------------------------------------------*/

void
CStandardPSPEventHandler::OnPatchSetStart( const CORE::CString& patchSetName )
{TRACE;
        // no need to do anything
}

/*-------------------------------------------------------------------------*/
    
void
CStandardPSPEventHandler::OnEnterLocalDir( const CORE::CString& localPath )
{TRACE;
        // the directory should exist locally
        CORE::Create_Directory( localPath.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CStandardPSPEventHandler::OnLocalFileOK( const CORE::CString& localPath ,
                                         const CORE::CString& localFile )
{TRACE;
        // no need to do anything
}

/*-------------------------------------------------------------------------*/

void
CStandardPSPEventHandler::OnLocalFileNotFound( const CORE::CString& localPath ,
                                               const CORE::CString& localFile )
{TRACE;
        // no need to do anything
}

/*-------------------------------------------------------------------------*/

void
CStandardPSPEventHandler::OnLocalFileDifference( const CORE::CString& localPath ,
                                                 const CORE::CString& localFile )
{TRACE;
        // no need to do anything
}

/*-------------------------------------------------------------------------*/

void
CStandardPSPEventHandler::OnNewSourceRequired( const TSourceInfo& sourceInfo )
{TRACE;
        m_sourceList[ sourceInfo.Tag ] = sourceInfo;
}

/*-------------------------------------------------------------------------*/
    
void
CStandardPSPEventHandler::OnLeaveLocalDir( const CORE::CString& localPath )
{TRACE;
}

/*-------------------------------------------------------------------------*/
    
void
CStandardPSPEventHandler::OnPatchSetEnd( const CORE::CString& patchSetName )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void
CStandardPSPEventHandler::OnParserError( void )
{TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
