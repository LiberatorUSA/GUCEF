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

-- Configuration for module: gucefPATCHERAPP


project( "gucefPATCHERAPP" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C++" )



configuration( {} )
kind( "SharedLib" )

configuration( {} )
links( { "gucefCOM", "gucefCOMCORE", "gucefCORE", "gucefGUI", "gucefIMAGE", "gucefINPUT", "gucefMT", "gucefPATCHER" } )
links( { "gucefCOM", "gucefCOMCORE", "gucefCORE", "gucefGUI", "gucefIMAGE", "gucefINPUT", "gucefMT", "gucefPATCHER" } )


configuration( {} )
defines( { "GUCEF_PATCHERAPP_BUILD_MODULE" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "include/gucefPATCHERAPP_CMainPatcherAppLogic.h",
  "include/gucefPATCHERAPP_CPatcherAppConfig.h",
  "include/gucefPATCHERAPP_CPatcherAppGlobal.h",
  "include/gucefPATCHERAPP_SimpleTypes.h",
  "include/gucefPATCHERAPP_config.h",
  "include/gucefPATCHERAPP_macros.h",
  "include/gucefPATCHERAPP_main.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src/gucefPATCHERAPP_CMainPatcherAppLogic.cpp",
  "src/gucefPATCHERAPP_CPatcherAppConfig.cpp",
  "src/gucefPATCHERAPP_CPatcherAppGlobal.cpp",
  "src/gucefPATCHERAPP_main.cpp"
 } )


configuration( {} )
includedirs( { "../../common/include", "../../platform/gucefCOM/include", "../../platform/gucefCOMCORE/include", "../../platform/gucefCORE/include", "../../platform/gucefGUI/include", "../../platform/gucefIMAGE/include", "../../platform/gucefINPUT/include", "../../platform/gucefMT/include", "../../platform/gucefPATCHER/include", "../../platform/gucefVFS/include", "include" } )

configuration( { "ANDROID" } )
includedirs( { "../../platform/gucefCORE/include/android" } )

configuration( { "LINUX32" } )
includedirs( { "../../platform/gucefCORE/include/linux" } )

configuration( { "LINUX64" } )
includedirs( { "../../platform/gucefCORE/include/linux" } )

configuration( { "WIN32" } )
includedirs( { "../../platform/gucefCOMCORE/include/mswin", "../../platform/gucefCORE/include/mswin" } )

configuration( { "WIN64" } )
includedirs( { "../../platform/gucefCOMCORE/include/mswin", "../../platform/gucefCORE/include/mswin" } )