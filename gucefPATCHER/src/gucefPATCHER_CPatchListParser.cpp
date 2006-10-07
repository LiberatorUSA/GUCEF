/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#include "gucefPATCHER_CPatchListParser.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CPatchListParser::PatchListRetrievalBeginEvent = "GUCEF::PATCHER::CPatchListParser::PatchListRetrievalBeginEvent";
const CORE::CString CPatchListParser::PatchListRetrievalEndEvent = "GUCEF::PATCHER::CPatchListParser::PatchListRetrievalEndEvent";
const CORE::CString CPatchListParser::PatchListRetrievalErrorEvent = "GUCEF::PATCHER::CPatchListParser::PatchListRetrievalErrorEvent";
const CORE::CString CPatchListParser::PatchListEntryEvent = "GUCEF::PATCHER::CPatchListParser::PatchListEntryEvent";
const CORE::CString CPatchListParser::PatchSetEntryEvent = "GUCEF::PATCHER::CPatchListParser::PatchSetEntryEvent";
const CORE::CString CPatchListParser::PatchSetRetrievalBeginEvent = "GUCEF::PATCHER::CPatchListParser::PatchSetRetrievalBeginEvent";
const CORE::CString CPatchListParser::PatchSetRetrievalEndEvent = "GUCEF::PATCHER::CPatchListParser::PatchSetRetrievalEndEvent";
const CORE::CString CPatchListParser::PatchSetRetrievalErrorEvent = "GUCEF::PATCHER::CPatchListParser::PatchSetRetrievalErrorEvent";
const CORE::CString CPatchListParser::ParserErrorEvent = "GUCEF::PATCHER::CPatchListParser::ParserErrorEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchListParser::CPatchListParser( void )
    : CORE::CObservingNotifier()                                       ,
      m_patchListRetrievalBeginEventID( PatchListRetrievalBeginEvent ) ,
      m_patchListRetrievalEndEventID( PatchListRetrievalEndEvent )     ,
      m_patchListRetrievalErrorEventID( PatchListRetrievalErrorEvent ) ,
      m_patchListEntryEventID( PatchListEntryEvent )                   ,
      m_patchSetEntryEventID( PatchSetEntryEvent )                     ,
      m_patchSetRetrievalBeginEventID( PatchSetRetrievalBeginEvent )   ,
      m_patchSetRetrievalEndEventID( PatchSetRetrievalEndEvent )       ,
      m_patchSetRetrievalErrorEventID( PatchSetRetrievalErrorEvent )   ,
      m_parserErrorEventID( ParserErrorEvent )                         ,
      m_url()                                                          ,
      m_isActive( false )                                              ,
      m_retrievingList( false )
{TRACE;

    SubscribeTo( &m_url );
}

/*-------------------------------------------------------------------------*/

CPatchListParser::~CPatchListParser()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPatchListParser::ProcessPatchList( const CORE::CDataNode& patchList )
{TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPatchListParser::ProcessPatchList( const CORE::CString& patchListURL )
{TRACE;
    
    if ( !m_isActive )
    {
        if ( m_url.SetURL( patchListURL ) )
        {
            if ( m_url.Activate() )
            {
                m_isActive = true;
                m_retrievingList = true;
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPatchListParser::OnNotify( CORE::CNotifier* notifier                 ,
                            const CORE::CEvent& eventid               ,
                            CORE::CICloneable* eventdata /* = NULL */ )
{TRACE;

    // Don't process async notifications if we are not active
    if ( m_isActive )
    {
        // Only process messages from our own URL handler
        if ( notifier == &m_url )
        {
            if ( eventid == m_url.GetURLDataRetrievalErrorEventID() )
            {
                if ( m_retrievingList )
                {
                    NotifyObservers( m_patchListRetrievalErrorEventID );
                }
                else
                {
                    NotifyObservers( m_patchSetRetrievalErrorEventID );
                }
                
                Stop();
                return;
            }
           // if ( eventid ==  )
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CPatchListParser::IsActive( void ) const
{TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/

void
CPatchListParser::Stop( void )
{TRACE;

    if ( m_isActive )
    {
        m_url.Deactivate();
        m_isActive = false;
        m_retrievingList = false;
    }
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchListEntryEventID( void ) const
{TRACE;
    return m_patchListEntryEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchSetEntryEventID( void ) const
{TRACE;
    return m_patchSetEntryEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchSetRetrievalBeginEventID( void ) const
{TRACE;
    return m_patchSetRetrievalBeginEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchSetRetrievalEndEventID( void ) const
{TRACE;
    return m_patchSetRetrievalEndEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchSetRetrievalErrorEventID( void ) const
{TRACE;
    return m_patchSetRetrievalErrorEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetParserErrorEventID( void ) const
{TRACE;
    return m_parserErrorEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchListRetrievalBeginEventID( void ) const
{TRACE;
    return m_patchListRetrievalBeginEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchListRetrievalEndEventID( void ) const
{TRACE;
    return m_patchListRetrievalEndEventID;
}

/*-------------------------------------------------------------------------*/

CORE::CEvent
CPatchListParser::GetPatchListRetrievalErrorEventID( void ) const
{TRACE;
    return m_patchListRetrievalErrorEventID;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace PATCHER */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/