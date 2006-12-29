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

#ifndef GUCEF_PATCHER_CPATCHENGINE_H
#define GUCEF_PATCHER_CPATCHENGINE_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CURL_H
#include "CURL.h"
#define GUCEF_CORE_CURL_H
#endif /* GUCEF_CORE_CURL_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_PATCHER_CPATCHLISTPARSER_H
#include "gucefPATCHER_CPatchListParser.h"
#define GUCEF_PATCHER_CPATCHLISTPARSER_H
#endif /* GUCEF_PATCHER_CPATCHLISTPARSER_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETPARSER_H
#include "gucefPATCHER_CPatchSetParser.h"
#define GUCEF_PATCHER_CPATCHSETPARSER_H
#endif /* GUCEF_PATCHER_CPATCHSETPARSER_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
#include "gucefPATCHER_CPatchSetFileEngineEvents.h"
#define GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H
#endif /* GUCEF_PATCHER_CPATCHSETFILEENGINEEVENTS_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H
#include "gucefPATCHER_CPatchSetDirEngineEvents.h"
#define GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H
#endif /* GUCEF_PATCHER_CPATCHSETDIRENGINEEVENTS_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETENGINEEVENTS_H
#include "gucefPATCHER_CPatchSetEngineEvents.h"
#define GUCEF_PATCHER_CPATCHSETENGINEEVENTS_H
#endif /* GUCEF_PATCHER_CPATCHSETENGINEEVENTS_H ? */

#ifndef GUCEF_PATCHER_CPATCHLISTENGINEEVENTS_H
#include "gucefPATCHER_CPatchListEngineEvents.h"
#define GUCEF_PATCHER_CPATCHLISTENGINEEVENTS_H
#endif /* GUCEF_PATCHER_CPATCHLISTENGINEEVENTS_H ? */

#ifndef GUCEF_PATCHER_MACROS_H
#include "gucefPATCHER_macros.h"
#define GUCEF_PATCHER_MACROS_H
#endif /* GUCEF_PATCHER_MACROS_H ? */

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

class CPatchListEngine;

/*-------------------------------------------------------------------------*/

/**
 *  Top-level engine for performing patching tasks
 *  This class is intended as a RAD component
 */
class EXPORT_CPP CPatchEngine : public CORE::CObservingNotifier  ,
                                public CORE::CIConfigurable      , /* interface */
                                public CPatchSetDirEngineEvents  , /* event interface */
                                public CPatchSetFileEngineEvents , /* event interface */
                                public CPatchSetEngineEvents     , /* event interface */
                                public CPatchListEngineEvents      /* event interface */
{
    public:
    
    static const CORE::CEvent PatchProcessStartedEvent;
    static const CORE::CEvent PatchProcessCompletedEvent;
    static const CORE::CEvent PatchProcessAbortedEvent;
    static const CORE::CEvent PatchProcessFailedEvent;
    static const CORE::CEvent PatchListDataReceivedEvent;
    static const CORE::CEvent PatchListRetrievalStartedEvent;
    static const CORE::CEvent PatchListRetrievalCompletedEvent;
    static const CORE::CEvent PatchListRetrievalFailedEvent;
    static const CORE::CEvent PatchListRetrievalAbortedEvent;
    static const CORE::CEvent PatchListDecodingFailedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    CPatchEngine( void );
    virtual ~CPatchEngine();
    
    /**
     *  Starts the engine using the current configuration
     */
    bool Start( void );
    
    void Stop( void );
    
    bool IsActive( void ) const;
    
    virtual bool SaveConfig( CDataNode& tree );
                                   
    virtual bool LoadConfig( const CDataNode& treeroot );
    
    bool AddEngineStartTriggerEvent( const CORE::CEvent& triggerEvent );   
    
    bool RemoveEngineStartTriggerEvent( const CORE::CEvent& triggerEvent );
    
    bool AddEngineStopTriggerEvent( const CORE::CEvent& triggerEvent );   
    
    bool RemoveEngineStopTriggerEvent( const CORE::CEvent& triggerEvent );    

    bool SetLocalRootDir( const CORE::CString& localRoot );
    
    const CORE::CString& GetLocalRootDir( void ) const;
    
    bool SetLocalTempStorageRootDir( const CORE::CString& tempStorageRoot );
    
    const CORE::CString& GetLocalTempStorageRootDir( void ) const;
    
    bool SetPatchListURL( const CORE::CString& patchListURL );
    
    const CORE::CString& GetPatchListURL( void ) const;

    bool SetPatchListCodec( const CORE::CString& patchListCodec );
    
    const CORE::CString& GetPatchListCodec( void ) const;

    protected:
        
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );

    private:
    
    CPatchEngine( const CPatchEngine& src );            /**< not implemented */
    CPatchEngine& operator=( const CPatchEngine& src ); /**< not implemented */
    bool ProcessRecievedPatchList( void );
    
    private:
    
    CPatchListEngine* m_patchListEngine;
    CORE::CURL m_url;
    CORE::CDynamicBuffer m_listDataBuffer;
    bool m_isActive;
    bool m_stopSignalGiven;
    CORE::CString m_patchListURL;
    CORE::CString m_patchListCodec;
    CORE::CString m_localRoot;
    CORE::CString m_tempStorageRoot;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CPATCHENGINE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-12-2006 :
        - Dinand: Initial version

-----------------------------------------------------------------------------*/