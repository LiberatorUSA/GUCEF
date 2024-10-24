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

-- Configuration for module: comcorepluginDBL


project( "comcorepluginDBL" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C++" )



configuration( {} )
kind( "SharedLib" )

configuration( {} )
links( { "gucefCOMCORE", "gucefCORE", "gucefMT" } )
links( { "gucefCOMCORE", "gucefCORE", "gucefMT" } )


configuration( {} )
defines( { "COMCOREPLUGIN_DBL_BUILD_PLUGIN_DLL" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "include/comcorepluginDBL.h",
  "include/comcorepluginDBL_CDBLNetworkInterface.h",
  "include/comcorepluginDBL_config.h",
  "include/comcorepluginDBL_driverapi.h",
  "include/comcorepluginDBL_macros.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src/comcorepluginDBL.cpp",
  "src/comcorepluginDBL_CDBLNetworkInterface.cpp",
  "src/comcorepluginDBL_driverapi.cpp"
 } )


configuration( {} )
includedirs( { "../../../common/include", "../../../platform/gucefCOMCORE/include", "../../../platform/gucefCORE/include", "../../../platform/gucefCORE/include/c_api", "../../../platform/gucefMT/include", "include" } )

configuration( { "ANDROID" } )
includedirs( { "../../../platform/gucefCORE/include/android" } )

configuration( { "LINUX32" } )
includedirs( { "../../../platform/gucefCOMCORE/include/linux", "../../../platform/gucefCORE/include/linux" } )

configuration( { "LINUX64" } )
includedirs( { "../../../platform/gucefCOMCORE/include/linux", "../../../platform/gucefCORE/include/linux" } )

configuration( { "WIN32" } )
includedirs( { "../../../platform/gucefCOMCORE/include/mswin", "../../../platform/gucefCORE/include/mswin", "../../../platform/gucefMT/include/mswin" } )

configuration( { "WIN64" } )
includedirs( { "../../../platform/gucefCOMCORE/include/mswin", "../../../platform/gucefCORE/include/mswin", "../../../platform/gucefMT/include/mswin" } )
