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

-- Configuration for module: guidriverCEGUI


project( "guidriverCEGUI" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C++" )



configuration( {} )
kind( "SharedLib" )

configuration( {} )
links( { "CEGUI", "freetype", "gucefCORE", "gucefGUI", "gucefINPUT", "gucefMT", "gucefVFS" } )
links( { "CEGUI", "freetype", "gucefCORE", "gucefGUI", "gucefINPUT", "gucefMT", "gucefVFS" } )


configuration( {} )
defines( { "GUIDRIVERCEGUI_BUILD_MODULE" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "include/guidriverCEGUI.h",
  "include/guidriverCEGUI_CButtonImp.h",
  "include/guidriverCEGUI_CCEGUIInputAdapter.h",
  "include/guidriverCEGUI_CCEGuiDriver.h",
  "include/guidriverCEGUI_CCheckboxImp.h",
  "include/guidriverCEGUI_CComboboxImp.h",
  "include/guidriverCEGUI_CEditboxImp.h",
  "include/guidriverCEGUI_CFileOpenDialogImp.h",
  "include/guidriverCEGUI_CFileSaveDialogImp.h",
  "include/guidriverCEGUI_CFileSystemDialogImp.h",
  "include/guidriverCEGUI_CFormBackendImp.h",
  "include/guidriverCEGUI_CGUIContext.h",
  "include/guidriverCEGUI_CGridViewImp.h",
  "include/guidriverCEGUI_CImageFrameImp.h",
  "include/guidriverCEGUI_CLabelImp.h",
  "include/guidriverCEGUI_CListBoxImp.h",
  "include/guidriverCEGUI_CLogAdapter.h",
  "include/guidriverCEGUI_CMemoboxImp.h",
  "include/guidriverCEGUI_CMenuBarImp.h",
  "include/guidriverCEGUI_CModule.h",
  "include/guidriverCEGUI_CPopupMenuImp.h",
  "include/guidriverCEGUI_CProgressBarImp.h",
  "include/guidriverCEGUI_CPushButtonImp.h",
  "include/guidriverCEGUI_CRenderContextImp.h",
  "include/guidriverCEGUI_CSpinnerImp.h",
  "include/guidriverCEGUI_CTabContentPaneImp.h",
  "include/guidriverCEGUI_CTabControlImp.h",
  "include/guidriverCEGUI_CTreeviewImp.h",
  "include/guidriverCEGUI_CVFSInfoProvider.h",
  "include/guidriverCEGUI_CWidgetImp.h",
  "include/guidriverCEGUI_CWindowImp.h",
  "include/guidriverCEGUI_ETypes.h",
  "include/guidriverCEGUI_ImageCodecAdapter.h",
  "include/guidriverCEGUI_VFSResourceProvider.h",
  "include/guidriverCEGUI_XMLParserAdapter.h",
  "include/guidriverCEGUI_config.h",
  "include/guidriverCEGUI_macros.h",
  "include/guidriverCEGUI_pluginAPI.h",
  "include/guidriverCEGUI_widgets.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src/guidriverCEGUI.cpp",
  "src/guidriverCEGUI_CButtonImp.cpp",
  "src/guidriverCEGUI_CCEGUIInputAdapter.cpp",
  "src/guidriverCEGUI_CCEGuiDriver.cpp",
  "src/guidriverCEGUI_CCheckboxImp.cpp",
  "src/guidriverCEGUI_CComboboxImp.cpp",
  "src/guidriverCEGUI_CEditboxImp.cpp",
  "src/guidriverCEGUI_CFileOpenDialogImp.cpp",
  "src/guidriverCEGUI_CFileSaveDialogImp.cpp",
  "src/guidriverCEGUI_CFileSystemDialogImp.cpp",
  "src/guidriverCEGUI_CFormBackendImp.cpp",
  "src/guidriverCEGUI_CGUIContext.cpp",
  "src/guidriverCEGUI_CGridViewImp.cpp",
  "src/guidriverCEGUI_CImageFrameImp.cpp",
  "src/guidriverCEGUI_CLabelImp.cpp",
  "src/guidriverCEGUI_CListboxImp.cpp",
  "src/guidriverCEGUI_CLogAdapter.cpp",
  "src/guidriverCEGUI_CMemoboxImp.cpp",
  "src/guidriverCEGUI_CMenuBarImp.cpp",
  "src/guidriverCEGUI_CModule.cpp",
  "src/guidriverCEGUI_CPopupMenuImp.cpp",
  "src/guidriverCEGUI_CProgressBarImp.cpp",
  "src/guidriverCEGUI_CPushButtonImp.cpp",
  "src/guidriverCEGUI_CRenderContextImp.cpp",
  "src/guidriverCEGUI_CSpinnerImp.cpp",
  "src/guidriverCEGUI_CTabContentPaneImp.cpp",
  "src/guidriverCEGUI_CTabControlImp.cpp",
  "src/guidriverCEGUI_CTreeviewImp.cpp",
  "src/guidriverCEGUI_CVFSInfoProvider.cpp",
  "src/guidriverCEGUI_CWidgetImp.cpp",
  "src/guidriverCEGUI_CWindowImp.cpp",
  "src/guidriverCEGUI_ImageCodecAdapter.cpp",
  "src/guidriverCEGUI_VFSResourceProvider.cpp",
  "src/guidriverCEGUI_XMLParserAdapter.cpp",
  "src/guidriverCEGUI_pluginAPI.cpp",
  "src/guidriverCEGUI_widgets.cpp"
 } )


configuration( {} )
includedirs( { "../../../common/include", "../../../dependencies/cegui/cegui/include", "../../../dependencies/freetype/include", "../../../dependencies/freetype/include/freetype", "../../../dependencies/freetype/include/freetype/config", "../../../dependencies/freetype/include/freetype/internal", "../../../dependencies/freetype/include/freetype/internal/services", "../../../dependencies/freetype/src", "../../../dependencies/freetype/src/winfonts", "../../../platform/gucefCORE/include", "../../../platform/gucefGUI/include", "../../../platform/gucefIMAGE/include", "../../../platform/gucefINPUT/include", "../../../platform/gucefMT/include", "../../../platform/gucefVFS/include", "include" } )

configuration( { "ANDROID" } )
includedirs( { "../../../platform/gucefCORE/include/android" } )

configuration( { "LINUX32" } )
includedirs( { "../../../platform/gucefCORE/include/linux" } )

configuration( { "LINUX64" } )
includedirs( { "../../../platform/gucefCORE/include/linux" } )

configuration( { "WIN32" } )
includedirs( { "../../../platform/gucefCORE/include/mswin" } )

configuration( { "WIN64" } )
includedirs( { "../../../platform/gucefCORE/include/mswin" } )