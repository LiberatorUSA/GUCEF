--------------------------------------------------------------------
-- This file was automatically generated by ProjectGenerator
-- which is tooling part the build system designed for GUCEF
--     (Galaxy Unlimited Framework)
-- For the latest info, see http://www.VanvelzenSoftware.com/
--
-- The contents of this file are placed in the public domain. Feel
-- free to make use of it in any way you like.
--------------------------------------------------------------------
--

-- Configuration for module: gucefCORE


project( "gucefCORE" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C++" )



configuration( {} )
kind( "SharedLib" )

configuration( {} )
links( { "gucefMT" } )
links( { "gucefMT" } )


configuration( {} )
defines( { "GUCEF_CORE_BUILD_MODULE" } )
  links( { "dl" } )
  links( { "X11", "dl", "rt" } )
  links( { "X11", "dl", "rt" } )
  links( { "Ws2_32.lib" } )
  links( { "Ws2_32.lib" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "include/CBlockStack.h",
  "include/CCPPWrapFileAccess.h",
  "include/CCodecChain.h",
  "include/CCodecRegistry.h",
  "include/CConfigStore.h",
  "include/CCyclicDynamicBuffer.h",
  "include/CDStoreCodec.h",
  "include/CDStoreCodecPlugin.h",
  "include/CDStoreCodecPluginManager.h",
  "include/CDStoreCodecRegistry.h",
  "include/CDVString.h",
  "include/CDataNode.h",
  "include/CDataQueue.h",
  "include/CDynamicBuffer.h",
  "include/CDynamicBufferAccess.h",
  "include/CEvent.h",
  "include/CException.h",
  "include/CExclusiveActivationManager.h",
  "include/CExclusiveActiveObj.h",
  "include/CFileAccess.h",
  "include/CFileURLHandler.h",
  "include/CGUCEFApplication.h",
  "include/CGUCEFCOREModule.h",
  "include/CGenericPlugin.h",
  "include/CGenericPluginManager.h",
  "include/CICloneable.h",
  "include/CICodec.h",
  "include/CICodecPlugin.h",
  "include/CIConfigurable.h",
  "include/CILogger.h",
  "include/CINamedInstance.h",
  "include/CINumericIDGeneratorBase.h",
  "include/CIOAccess.h",
  "include/CIPlugin.h",
  "include/CIStreamable.h",
  "include/CISysConsoleCmdHandler.h",
  "include/CITypeNamed.h",
  "include/CIURLEvents.h",
  "include/CLLNode.h",
  "include/CLogManager.h",
  "include/CMFileAccess.h",
  "include/CMachineState.h",
  "include/CMemoryBlockPool.h",
  "include/CMsgException.h",
  "include/CNode.h",
  "include/CNotificationIDRegistry.h",
  "include/CNotifier.h",
  "include/CNotifierImplementor.h",
  "include/CNotifierObservingComponent.h",
  "include/CNotifyingMapEvents.h",
  "include/CObserver.h",
  "include/CObservingNotifier.h",
  "include/CPFileAccess.h",
  "include/CPFileChunkAccess.h",
  "include/CPPWrapFileAccess.h",
  "include/CPluginControl.h",
  "include/CPluginManager.h",
  "include/CPumpedObserver.h",
  "include/CROIOAccess.h",
  "include/CSTDOStreamAdapter.h",
  "include/CStack.h",
  "include/CStateHandler.h",
  "include/CStateMachine.h",
  "include/CStateMachineComponent.h",
  "include/CStdCodecPlugin.h",
  "include/CStdCodecPluginItem.h",
  "include/CStdCodecPluginManager.h",
  "include/CStdLogger.h",
  "include/CStreamerEvents.h",
  "include/CSysConsole.h",
  "include/CSysConsoleClient.h",
  "include/CTAbstractFactory.h",
  "include/CTAbstractFactoryWithParam.h",
  "include/CTBasicSharedPtr.h",
  "include/CTCloneableExpansion.h",
  "include/CTCloneableObj.h",
  "include/CTCodecPluginItem.h",
  "include/CTDynamicArray.h",
  "include/CTDynamicDestructor.h",
  "include/CTDynamicDestructorBase.h",
  "include/CTFactory.h",
  "include/CTFactoryBase.h",
  "include/CTFactoryBaseWithParam.h",
  "include/CTFactoryWithParam.h",
  "include/CTLinkedCloneableObj.h",
  "include/CTNotifyingMap.h",
  "include/CTNumericID.h",
  "include/CTNumericIDGenerator.h",
  "include/CTONRegistry.h",
  "include/CTObservingNotifierComponent.h",
  "include/CTObservingNotifierExpansion.h",
  "include/CTRegistry.h",
  "include/CTSGNotifier.h",
  "include/CTSGObserver.h",
  "include/CTSharedPtr.h",
  "include/CTSingleton.h",
  "include/CTStreamableObj.h",
  "include/CTStreamer.h",
  "include/CTStringStreamable.h",
  "include/CTThreadSafeExpansion.h",
  "include/CTimer.h",
  "include/CTracer.h",
  "include/CTransactionEventControl.h",
  "include/CURL.h",
  "include/CURLDataRetriever.h",
  "include/CURLHandler.h",
  "include/CURLHandlerRegistry.h",
  "include/CValueList.h",
  "include/CodecPluginLink.h",
  "include/ConfigStoreCodec.h",
  "include/DStoreCodecPluginStructs.h",
  "include/DVCPPOSWRAP.h",
  "include/DVOSWRAP.h",
  "include/EEnum.h",
  "include/EStructs.h",
  "include/ExceptionClassMacros.h",
  "include/ExceptionThrowMacros.h",
  "include/MFILE.h",
  "include/MemoryPool.h",
  "include/MemorySegment.h",
  "include/NumericIDs.h",
  "include/PAFILE.h",
  "include/callstack.h",
  "include/cloneables.h",
  "include/dvcppfileutils.h",
  "include/dvcppstringutils.h",
  "include/dvfileutils.h",
  "include/dvmd5utils.h",
  "include/dvstrutils.h",
  "include/gucefCORE.h",
  "include/gucefCORE_CAbstractFactoryBase.h",
  "include/gucefCORE_CBasicBracketLoggingFormatter.h",
  "include/gucefCORE_CBusyWaitPulseGeneratorDriver.h",
  "include/gucefCORE_CCharSepLoggingFormatter.h",
  "include/gucefCORE_CCoreGlobal.h",
  "include/gucefCORE_CDate.h",
  "include/gucefCORE_CDateTime.h",
  "include/gucefCORE_CDirectoryWatcher.h",
  "include/gucefCORE_CEventBasedTaskConsumer.h",
  "include/gucefCORE_CForwardingNotifier.h",
  "include/gucefCORE_CIConsoleLogger.h",
  "include/gucefCORE_CIDate.h",
  "include/gucefCORE_CIEventHandlerFunctorBase.h",
  "include/gucefCORE_CILoggingFormatter.h",
  "include/gucefCORE_CIMetricsSystemClient.h",
  "include/gucefCORE_CIOAccessToIOStream.h",
  "include/gucefCORE_CIObserver.h",
  "include/gucefCORE_CIPluginLoadLogic.h",
  "include/gucefCORE_CIPluginMetaData.h",
  "include/gucefCORE_CIPulseGeneratorDriver.h",
  "include/gucefCORE_CITaskConsumer.h",
  "include/gucefCORE_CITime.h",
  "include/gucefCORE_CIniDataStoreCodec.h",
  "include/gucefCORE_CIniParser.h",
  "include/gucefCORE_CJsonLoggingFormatter.h",
  "include/gucefCORE_CLinkedTransferBuffer.h",
  "include/gucefCORE_CLoaderDelegatedPluginLoadLogic.h",
  "include/gucefCORE_CLoggingTask.h",
  "include/gucefCORE_CMetricsClientManager.h",
  "include/gucefCORE_CMultiLogger.h",
  "include/gucefCORE_COSWindow.h",
  "include/gucefCORE_CObserverSwitch.h",
  "include/gucefCORE_CPlatformNativeConsoleLogger.h",
  "include/gucefCORE_CPlatformNativeConsoleWindow.h",
  "include/gucefCORE_CPluginGroup.h",
  "include/gucefCORE_CPluginMetaData.h",
  "include/gucefCORE_CPulseData.h",
  "include/gucefCORE_CPulseGenerator.h",
  "include/gucefCORE_CRollingFileAccess.h",
  "include/gucefCORE_CSimplisticPluginLoadLogic.h",
  "include/gucefCORE_CSingleTaskDelegator.h",
  "include/gucefCORE_CSubFileAccess.h",
  "include/gucefCORE_CTEventHandlerFunctor.h",
  "include/gucefCORE_CTaskDelegator.h",
  "include/gucefCORE_CTaskManager.h",
  "include/gucefCORE_CTime.h",
  "include/gucefCORE_CVersion.h",
  "include/gucefCORE_CVersionRange.h",
  "include/gucefCORE_CoreCodecTypes.h",
  "include/gucefCORE_ETypes.h",
  "include/gucefCORE_GenericPluginAPI.h",
  "include/gucefCORE_LogLevels.h",
  "include/gucefCORE_LogTypes.h",
  "include/gucefCORE_Logging.h",
  "include/gucefCORE_LoggingMacros.h",
  "include/gucefCORE_MetricsMacros.h",
  "include/gucefCORE_cinterface.h",
  "include/gucefCORE_config.h",
  "include/gucefCORE_macros.h",
  "include/gucef_essentials.h",
  "include/gucef_new_off.h",
  "include/gucef_new_on.h",
  "include/ioaccess.h",
  "include/macros.h",
  "include/md5.h",
  "include/streamables.h",
  "include/streamers.h",
  "include/tsprinting.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src/CBlockStack.cpp",
  "src/CCPPWrapFileAccess.cpp",
  "src/CCodecChain.cpp",
  "src/CCodecRegistry.cpp",
  "src/CConfigStore.cpp",
  "src/CCyclicDynamicBuffer.cpp",
  "src/CDStoreCodec.cpp",
  "src/CDStoreCodecPlugin.cpp",
  "src/CDStoreCodecPluginManager.cpp",
  "src/CDStoreCodecRegistry.cpp",
  "src/CDVString.cpp",
  "src/CDataNode.cpp",
  "src/CDataQueue.cpp",
  "src/CDynamicBuffer.cpp",
  "src/CDynamicBufferAccess.cpp",
  "src/CEvent.cpp",
  "src/CException.cpp",
  "src/CExclusiveActivationManager.cpp",
  "src/CExclusiveActiveObj.cpp",
  "src/CFileAccess.cpp",
  "src/CFileURLHandler.cpp",
  "src/CGUCEFApplication.cpp",
  "src/CGUCEFCOREModule.cpp",
  "src/CGenericPlugin.cpp",
  "src/CGenericPluginManager.cpp",
  "src/CICloneable.cpp",
  "src/CICodec.cpp",
  "src/CICodecPlugin.cpp",
  "src/CIConfigurable.cpp",
  "src/CILogger.cpp",
  "src/CINamedInstance.cpp",
  "src/CINumericIDGeneratorBase.cpp",
  "src/CIOAccess.cpp",
  "src/CIPlugin.cpp",
  "src/CIStreamable.cpp",
  "src/CITypeNamed.cpp",
  "src/CIURLEvents.cpp",
  "src/CLLNode.cpp",
  "src/CLogManager.cpp",
  "src/CMFileAccess.cpp",
  "src/CMachineState.cpp",
  "src/CMemoryBlockPool.cpp",
  "src/CMsgException.cpp",
  "src/CNode.cpp",
  "src/CNotificationIDRegistry.cpp",
  "src/CNotifier.cpp",
  "src/CNotifierImplementor.cpp",
  "src/CNotifierObservingComponent.cpp",
  "src/CNotifyingMapEvents.cpp",
  "src/CObserver.cpp",
  "src/CObservingNotifier.cpp",
  "src/CPFileAccess.cpp",
  "src/CPFileChunkAccess.cpp",
  "src/CPluginControl.cpp",
  "src/CPluginManager.cpp",
  "src/CPumpedObserver.cpp",
  "src/CROIOAccess.cpp",
  "src/CSTDOStreamAdapter.cpp",
  "src/CStack.cpp",
  "src/CStateHandler.cpp",
  "src/CStateMachine.cpp",
  "src/CStateMachineComponent.cpp",
  "src/CStdCodecPlugin.cpp",
  "src/CStdCodecPluginItem.cpp",
  "src/CStdCodecPluginManager.cpp",
  "src/CStdLogger.cpp",
  "src/CStreamerEvents.cpp",
  "src/CSysConsole.cpp",
  "src/CSysConsoleClient.cpp",
  "src/CTAbstractFactory.cpp",
  "src/CTAbstractFactoryWithParam.cpp",
  "src/CTBasicSharedPtr.cpp",
  "src/CTCloneableObj.cpp",
  "src/CTCodecPluginItem.cpp",
  "src/CTDynamicDestructor.cpp",
  "src/CTDynamicDestructorBase.cpp",
  "src/CTFactory.cpp",
  "src/CTFactoryBase.cpp",
  "src/CTFactoryBaseWithParam.cpp",
  "src/CTFactoryWithParam.cpp",
  "src/CTLinkedCloneableObj.cpp",
  "src/CTNotifyingMap.cpp",
  "src/CTNumericID.cpp",
  "src/CTNumericIDGenerator.cpp",
  "src/CTONRegistry.cpp",
  "src/CTObservingNotifierComponent.cpp",
  "src/CTObservingNotifierExpansion.cpp",
  "src/CTRegistry.cpp",
  "src/CTSGNotifier.cpp",
  "src/CTSGObserver.cpp",
  "src/CTSharedPtr.cpp",
  "src/CTSingleton.cpp",
  "src/CTimer.cpp",
  "src/CTracer.cpp",
  "src/CTransactionEventControl.cpp",
  "src/CURL.cpp",
  "src/CURLDataRetriever.cpp",
  "src/CURLHandler.cpp",
  "src/CURLHandlerRegistry.cpp",
  "src/CValueList.cpp",
  "src/DVCPPOSWRAP.cpp",
  "src/DVOSWRAP.cpp",
  "src/IOACCESS.c",
  "src/MFILE.c",
  "src/MemoryPool.cpp",
  "src/MemorySegment.cpp",
  "src/PAFILE.c",
  "src/callstack.c",
  "src/cloneables.cpp",
  "src/dvcppfileutils.cpp",
  "src/dvcppstringutils.cpp",
  "src/dvfileutils.c",
  "src/dvmd5utils.c",
  "src/dvstrutils.c",
  "src/gucefCORE.cpp",
  "src/gucefCORE_CAbstractFactoryBase.cpp",
  "src/gucefCORE_CBasicBracketLoggingFormatter.cpp",
  "src/gucefCORE_CBusyWaitPulseGeneratorDriver.cpp",
  "src/gucefCORE_CCharSepLoggingFormatter.cpp",
  "src/gucefCORE_CCoreGlobal.cpp",
  "src/gucefCORE_CDate.cpp",
  "src/gucefCORE_CDateTime.cpp",
  "src/gucefCORE_CDirectoryWatcher.cpp",
  "src/gucefCORE_CForwardingNotifier.cpp",
  "src/gucefCORE_CIConsoleLogger.cpp",
  "src/gucefCORE_CIDate.cpp",
  "src/gucefCORE_CIEventHandlerFunctorBase.cpp",
  "src/gucefCORE_CILoggingFormatter.cpp",
  "src/gucefCORE_CIMetricsSystemClient.cpp",
  "src/gucefCORE_CIOAccessToIOStream.cpp",
  "src/gucefCORE_CIObserver.cpp",
  "src/gucefCORE_CIPluginLoadLogic.cpp",
  "src/gucefCORE_CIPluginMetaData.cpp",
  "src/gucefCORE_CIPulseGeneratorDriver.cpp",
  "src/gucefCORE_CITaskConsumer.cpp",
  "src/gucefCORE_CITime.cpp",
  "src/gucefCORE_CIniDataStoreCodec.cpp",
  "src/gucefCORE_CIniParser.cpp",
  "src/gucefCORE_CJsonLoggingFormatter.cpp",
  "src/gucefCORE_CLinkedTransferBuffer.cpp",
  "src/gucefCORE_CLoaderDelegatedPluginLoadLogic.cpp",
  "src/gucefCORE_CLoggingTask.cpp",
  "src/gucefCORE_CMetricsClientManager.cpp",
  "src/gucefCORE_CMultiLogger.cpp",
  "src/gucefCORE_COSWindow.cpp",
  "src/gucefCORE_CObserverSwitch.cpp",
  "src/gucefCORE_CPlatformNativeConsoleLogger.cpp",
  "src/gucefCORE_CPlatformNativeConsoleWindow.cpp",
  "src/gucefCORE_CPluginGroup.cpp",
  "src/gucefCORE_CPluginMetaData.cpp",
  "src/gucefCORE_CPulseData.cpp",
  "src/gucefCORE_CPulseGenerator.cpp",
  "src/gucefCORE_CRollingFileAccess.cpp",
  "src/gucefCORE_CSimplisticPluginLoadLogic.cpp",
  "src/gucefCORE_CSingleTaskDelegator.cpp",
  "src/gucefCORE_CSubFileAccess.cpp",
  "src/gucefCORE_CTEventHandlerFunctor.cpp",
  "src/gucefCORE_CTaskDelegator.cpp",
  "src/gucefCORE_CTaskManager.cpp",
  "src/gucefCORE_CTime.cpp",
  "src/gucefCORE_CVersion.cpp",
  "src/gucefCORE_CVersionRange.cpp",
  "src/gucefCORE_CoreCodecTypes.cpp",
  "src/gucefCORE_LogLevels.cpp",
  "src/gucefCORE_cinterface.cpp",
  "src/md5.c",
  "src/streamables.cpp",
  "src/streamers.cpp",
  "src/tsprinting.c"
 } )



configuration( { "ANDROID" } )
    vpaths { ["Platform Headers"] = { "**.h", "**.hpp", "**.hxx" } }
    files( {
      "include/android/gucefCORE_CAndroidSystemLogger.h"
    } )

    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "src/android/gucefCORE_CAndroidSystemLogger.cpp"
    } )



configuration( { "LINUX32" } )
    vpaths { ["Platform Headers"] = { "**.h", "**.hpp", "**.hxx" } }
    files( {
      "include/linux/CXTermConsoleLogger.h",
      "include/linux/gucefCORE_CX11EventDispatcher.h",
      "include/linux/gucefCORE_CX11Window.h",
      "include/linux/gucefCORE_pty.h"
    } )

    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "src/linux/CXTermConsoleLogger.cpp",
      "src/linux/gucefCORE_CX11EventDispatcher.cpp",
      "src/linux/gucefCORE_CX11Window.cpp",
      "src/linux/gucefCORE_pty.c"
    } )



configuration( { "LINUX64" } )
    vpaths { ["Platform Headers"] = { "**.h", "**.hpp", "**.hxx" } }
    files( {
      "include/linux/CXTermConsoleLogger.h",
      "include/linux/gucefCORE_CX11EventDispatcher.h",
      "include/linux/gucefCORE_CX11Window.h",
      "include/linux/gucefCORE_pty.h"
    } )

    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "src/linux/CXTermConsoleLogger.cpp",
      "src/linux/gucefCORE_CX11EventDispatcher.cpp",
      "src/linux/gucefCORE_CX11Window.cpp",
      "src/linux/gucefCORE_pty.c"
    } )



configuration( { "WIN32" } )
    vpaths { ["Platform Headers"] = { "**.h", "**.hpp", "**.hxx" } }
    files( {
      "include/mswin/CMSWinConsoleLogger.h",
      "include/mswin/CWindowMsgHook.h",
      "include/mswin/CWndMsgHookNotifier.h",
      "include/mswin/gucefCORE_CMSWin32ConsoleWindow.h",
      "include/mswin/gucefCORE_CMsWin32Editbox.h",
      "include/mswin/gucefCORE_CMsWin32Window.h",
      "include/mswin/gucefCORE_mswinutils.h"
    } )

    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "src/mswin/CMSWinConsoleLogger.cpp",
      "src/mswin/CWindowMsgHook.cpp",
      "src/mswin/CWndMsgHookNotifier.cpp",
      "src/mswin/gucefCORE_CMSWin32ConsoleWindow.cpp",
      "src/mswin/gucefCORE_CMsWin32Editbox.cpp",
      "src/mswin/gucefCORE_CMsWin32Window.cpp",
      "src/mswin/gucefCORE_mswinutils.cpp"
    } )



configuration( { "WIN64" } )
    vpaths { ["Platform Headers"] = { "**.h", "**.hpp", "**.hxx" } }
    files( {
      "include/mswin/CMSWinConsoleLogger.h",
      "include/mswin/CWindowMsgHook.h",
      "include/mswin/CWndMsgHookNotifier.h",
      "include/mswin/gucefCORE_CMSWin32ConsoleWindow.h",
      "include/mswin/gucefCORE_CMsWin32Editbox.h",
      "include/mswin/gucefCORE_CMsWin32Window.h",
      "include/mswin/gucefCORE_mswinutils.h"
    } )

    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "src/mswin/CMSWinConsoleLogger.cpp",
      "src/mswin/CWindowMsgHook.cpp",
      "src/mswin/CWndMsgHookNotifier.cpp",
      "src/mswin/gucefCORE_CMSWin32ConsoleWindow.cpp",
      "src/mswin/gucefCORE_CMsWin32Editbox.cpp",
      "src/mswin/gucefCORE_CMsWin32Window.cpp",
      "src/mswin/gucefCORE_mswinutils.cpp"
    } )


configuration( {} )
includedirs( { "../../common/include", "../gucefMT/include", "include" } )

configuration( { "ANDROID" } )
includedirs( { "include/android" } )

configuration( { "LINUX32" } )
includedirs( { "include/linux" } )

configuration( { "LINUX64" } )
includedirs( { "include/linux" } )

configuration( { "WIN32" } )
includedirs( { "include/mswin" } )

configuration( { "WIN64" } )
includedirs( { "include/mswin" } )