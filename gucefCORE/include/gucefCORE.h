/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

/*
 *      This is the main include header for the gucefCORE library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do core framework functionality
 *      and many of the other modules will have a dependancy on this module.
 *      It houses the generic toolkits.
 */

#ifndef GUCEF_CORE_H
#define GUCEF_CORE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_GUCEFCORE_CONFIG_H
#include "gucefCORE_config.h"
#define GUCEF_CORE_GUCEFCORE_CONFIG_H
#endif /* GUCEF_CORE_GUCEFCORE_CONFIG_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "dvoswrap.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_MFILE_H
#include "MFILE.h"
#define GUCEF_CORE_MFILE_H
#endif /* GUCEF_CORE_MFILE_H ? */

#ifndef GUCEF_CORE_PAFILE_H
#include "PAFILE.h"
#define GUCEF_CORE_PAFILE_H
#endif /* GUCEF_CORE_PAFILE_H ? */

#ifndef GUCEF_CORE_IOACCESS_H
#include "ioaccess.h"
#define GUCEF_CORE_IOACCESS_H
#endif /* GUCEF_CORE_IOACCESS_H ? */

#ifndef GUCEF_CORE_MEMORYMANAGER_H
#include "MemoryManager.h"
#define GUCEF_CORE_MEMORYMANAGER_H
#endif /* GUCEF_CORE_MEMORYMANAGER_H ? */

#ifndef GUCEF_CORE_CALLSTACK_H
#include "callstack.h"
#define GUCEF_CORE_CALLSTACK_H
#endif /* GUCEF_CORE_CALLSTACK_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#include "CGUCEFAppSubSystem.h"
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h" 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CSTRINGMAP_H
#include "CStringMap.h"
#define GUCEF_CORE_CSTRINGMAP_H
#endif /* GUCEF_CORE_CSTRINGMAP_H ? */

#ifndef GUCEF_CORE_CINTMAP_H
#include "CIntMap.h"
#define GUCEF_CORE_CINTMAP_H
#endif /* GUCEF_CORE_CINTMAP_H ? */

#ifndef GUCEF_CORE_COBSERVER_H
#include "CObserver.h"
#define GUCEF_CORE_COBSERVER_H
#endif /* GUCEF_CORE_COBSERVER_H ? */

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTRANSACTIONEVENTCONTROL_H
#include "CTransactionEventControl.h"
#define GUCEF_CORE_CTRANSACTIONEVENTCONTROL_H
#endif /* GUCEF_CORE_CTRANSACTIONEVENTCONTROL_H ? */

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#ifndef GUCEF_CORE_CGENERICPLUGINMANAGER_H
#include "CGenericPluginManager.h"
#define GUCEF_CORE_CGENERICPLUGINMANAGER_H
#endif /* GUCEF_CORE_CGENERICPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CSTACK_H
#include "CStack.h"
#define GUCEF_CORE_CSTACK_H
#endif /* GUCEF_CORE_CSTACK_H ? */

#ifndef GUCEF_CORE_CDATAQUEUE_H
#include "CDataQueue.h"
#define GUCEF_CORE_CDATAQUEUE_H
#endif /* GUCEF_CORE_CDATAQUEUE_H ? */

#ifndef GUCEF_CORE_CBLOCKSTACK_H
#include "CBlockStack.h"
#define GUCEF_CORE_CBLOCKSTACK_H
#endif /* GUCEF_CORE_CBLOCKSTACK_H ? */

#ifndef GUCEF_CORE_CMEMORYBLOCKPOOL_H
#include "CMemoryBlockPool.h"
#define GUCEF_CORE_CMEMORYBLOCKPOOL_H
#endif /* GUCEF_CORE_CMEMORYBLOCKPOOL_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CPROPERTYLIST_H
#include "CPropertyList.h"
#define GUCEF_CORE_CPROPERTYLIST_H
#endif /* GUCEF_CORE_CPROPERTYLIST_H ? */

#ifndef GUCEF_CORE_CNODE_H
#include "CNode.h"
#define GUCEF_CORE_CNODE_H
#endif /* GUCEF_CORE_CNODE_H ? */

#ifndef GUCEF_CORE_CLLNODE_H
#include "CLLNode.h"
#define GUCEF_CORE_CLLNODE_H
#endif /* GUCEF_CORE_CLLNODE_H ? */

#ifndef GUCEF_CORE_CSTRINGLIST_H
#include "CStringList.h"
#define GUCEF_CORE_CSTRINGLIST_H
#endif /* GUCEF_CORE_CSTRINGLIST_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneAble.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */ 

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#include "CPluginManager.h"
#define GUCEF_CORE_CPLUGINMANAGER_H
#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef GUCEF_CORE_CDSTORECODEC_H
#include "CDStoreCodec.h" 
#define GUCEF_CORE_CDSTORECODEC_H
#endif /* GUCEF_CORE_CDSTORECODEC_H */

#ifndef GUCEF_CORE_CDSTORECODECPLUGIN_H
#include "CDStoreCodecPlugin.h" 
#define GUCEF_CORE_CDSTORECODECPLUGIN_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGIN_H */

#ifndef GUCEF_CORE_CDSTORECODECPLUGINREF_H
#include "CDStoreCodecPluginRef.h" 
#define GUCEF_CORE_CDSTORECODECPLUGINREF_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGINREF_H */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#include "CDStoreCodecPluginManager.h" 
#define GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CTREGISTRY_H
#include "CTRegistry.h"
#define GUCEF_CORE_CTREGISTRY_H
#endif /* GUCEF_CORE_CTREGISTRY_H ? */

#ifndef GUCEF_CORE_CURL_H
#include "CURL.h"
#define GUCEF_CORE_CURL_H
#endif /* GUCEF_CORE_CURL_H ? */

#ifndef GUCEF_CORE_CURLHANDLER_H
#include "CURLHandler.h"
#define GUCEF_CORE_CURLHANDLER_H
#endif /* GUCEF_CORE_CURLHANDLER_H ? */

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

#ifndef GUCEF_CORE_CURLDATARETRIEVER_H
#include "CURLDataRetriever.h"
#define GUCEF_CORE_CURLDATARETRIEVER_H
#endif /* GUCEF_CORE_CURLDATARETRIEVER_H ? */

#ifndef GUCEF_CORE_CSTATEMACHINE_H
#include "CStateMachine.h"
#define GUCEF_CORE_CSTATEMACHINE_H
#endif /* GUCEF_CORE_CSTATEMACHINE_H ? */

#ifndef GUCEF_CORE_CSTATEHANDLER_H
#include "CStateHandler.h"
#define GUCEF_CORE_CSTATEHANDLER_H
#endif /* GUCEF_CORE_CSTATEHANDLER_H ? */

#ifndef GUCEF_CORE_CSTDOSTREAMADAPTER_H
#include "CSTDOStreamAdapter.h"
#define GUCEF_CORE_CSTDOSTREAMADAPTER_H
#endif /* GUCEF_CORE_CSTDOSTREAMADAPTER_H ? */

#ifndef GUCEF_CORE_CIPLUGIN_H
#include "CIPlugin.h"
#define GUCEF_CORE_CIPLUGIN_H
#endif /* GUCEF_CORE_CIPLUGIN_H ? */

#ifndef GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#include "CTLinkedCloneableObj.h"
#define GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CTBASICSHAREDPTR_H 
#include "CTBasicSharedPtr.h"
#define GUCEF_CORE_CTBASICSHAREDPTR_H
#endif /* GUCEF_CORE_CTBASICSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C const char* GUCEF_CORE_ModuleCompileTimeStamp( void );

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-10-2006 :
       - Dinand: Added CTLinkedCloneableObj
       - Dinand: re-Added CTCloneableObj
- 30-09-2006 :
       - Dinand: re-Added CGenericPluginManager
       - Dinand: re-Added CIPlugin
       - Dinand: Removed CEventPump
       - Dinand: Removed CEventPumpClient
       - Dinand: Removed CStringStorage
       - Dinand: Removed CTRegistryMT
- 21-09-2005 :
       - Dinand: Added CValueList
- 15-05-2005 :
       - Dinand: Added CStateMachine
       - Dinand: Added CStateHandler
- 24-04-2005 :
       - Dinand: Added CTRegistryMT
       - Dinand: Added CTRegistry
       - Dinand: Added CURL
       - Dinand: Added CURLHandler
       - Dinand: Added CURLHandlerRegistry
- 08-04-2005 :
       - Dinand: Added CConfigStore
       - Dinand: Added CDStoreCodec
       - Dinand: Added CDStoreCodecPlugin
       - Dinand: Added CDStoreCodecPluginRef
       - Dinand: Added CDStoreCodecRegistry
       - Dinand: Added CDStoreCodecPluginManager
- 05-04-2005 :
       - Dinand: Added CDataNode
       - Dinand: Removed CXMLSegment
       - Dinand: Removed CXMLParser
       - Dinand: Removed CXMLComment
       - Dinand: Removed CXMLNode
- 11-03-2004 :
       - Dinand: Made the initial version of the library header. The classes here
         where originally part of one large GUCEF codebase without segmentation.

-----------------------------------------------------------------------------*/
