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

#ifndef GUCEF_CORE_CURLDATARETRIEVER_H
#define GUCEF_CORE_CURLDATARETRIEVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CURL_H
#include "CURL.h"
#define GUCEF_CORE_CURL_H
#endif /* GUCEF_CORE_CURL_H ? */

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

/**
 *  class that allows you to retrieve and subsequently store data
 *  using URL's
 */
class GUCEFCORE_EXPORT_CPP CURLDataRetriever : public CObservingNotifier ,
                                               public CIURLEvents
{
    public:
    
    CURLDataRetriever( void );
    
    virtual ~CURLDataRetriever();
    
    CURL& GetURL( void );
    
    CDynamicBuffer& GetBuffer( void );
    
    CEvent GetURLActivateEventID( void ) const;
    CEvent GetURLDeactivateEventID( void ) const;
    CEvent GetURLDataRecievedEventID( void ) const;
    CEvent GetURLAllDataRecievedEventID( void ) const;
    CEvent GetURLDataRetrievalErrorEventID( void ) const;    

    protected:
    
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );
    
    private:
    
    CURLDataRetriever( const CURLDataRetriever& src );
    CURLDataRetriever& operator=( CURLDataRetriever& src );
    
    private:
    
    CDynamicBuffer m_buffer;
    CURL m_url;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CURLDATARETRIEVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2006 :
        - Initial implementation
          
---------------------------------------------------------------------------*/