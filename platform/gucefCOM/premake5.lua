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

-- Configuration for module: gucefCOM


project( "gucefCOM" )

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
defines( { "GUCEF_COM_BUILD_MODULE" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "include/CDBColumn.h",
  "include/CDBField.h",
  "include/CDBQuery.h",
  "include/CGUCEFCOMModule.h",
  "include/CMySQLClient.h",
  "include/CMySQLClientInterface.h",
  "include/CPHUDPSocket.h",
  "include/gucefCOM.h",
  "include/gucefCOMDLLInit.h",
  "include/gucefCOM_CComGlobal.h",
  "include/gucefCOM_CStatsDClient.h",
  "include/gucefCOM_ETypes.h",
  "include/gucefCOM_config.h",
  "include/gucefCOM_macros.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src/CDBQuery.cpp",
  "src/CGUCEFCOMModule.cpp",
  "src/CMySQLClient.cpp",
  "src/CPHUDPSocket.cpp",
  "src/gucefCOM.cpp",
  "src/gucefCOM_CComGlobal.cpp",
  "src/gucefCOM_CStatsDClient.cpp"
 } )


configuration( {} )
includedirs( { "../../common/include", "../gucefCOMCORE/include", "../gucefCORE/include", "../gucefMT/include", "include" } )

configuration( { "ANDROID" } )
includedirs( { "../gucefCORE/include/android" } )

configuration( { "LINUX32" } )
includedirs( { "../gucefCORE/include/linux" } )

configuration( { "LINUX64" } )
includedirs( { "../gucefCORE/include/linux" } )

configuration( { "WIN32" } )
includedirs( { "../gucefCOMCORE/include/mswin", "../gucefCORE/include/mswin" } )

configuration( { "WIN64" } )
includedirs( { "../gucefCOMCORE/include/mswin", "../gucefCORE/include/mswin" } )