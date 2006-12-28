/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2003.  All rights reserved.
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

#include "gucefPATCHER_CPatchSetEngine.h"	/* definition of this class */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchSetEngine::CPatchSetEngine( void )
{TRACE;

    SubscribeTo( &m_dataRetriever );
}

/*-------------------------------------------------------------------------*/

CPatchSetEngine::~CPatchSetEngine()
{TRACE;

    Stop();
}

/*-------------------------------------------------------------------------*/

bool
CPatchSetEngine::Start( const TPatchSet& patchSet ,
                        CORE::CString& localRoot  )
{TRACE;

    assert( &patchSet != NULL );
    
    // The user should explicitly stop first if we are already busy
    if ( !IsActive() )
    {
        // parameter sanity check
        if ( ( patchSet.size() > 1 )     &&
             ( localRoot.Length() > 0 )  )
        {
            m_isActive = true;
            m_stopSignalGiven = false;
            
            m_patchSet = &patchSet;
            m_curDir = *(m_patchSet->begin());
            m_parentDir = NULL;
            m_curFile = NULL;
            m_localRoot = localRoot;
            m_localPath = localRoot;
            
            ProcessCurDir();
            
            return true;
        }
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/
    
void
CPatchSetEngine::ProcessCurDir( void )
{TRACE;

    if ( m_curDir != NULL )
    {
        m_curDir->files
    }
}

/*-------------------------------------------------------------------------*/

void
CPatchSetEngine::Stop( void )
{TRACE;
    
    m_stopSignalGiven = true;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchSetEngine::IsActive( void ) bool
{TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/