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

-- Configuration for module: MFCPatcherGUI


configuration( { "WIN32" } )
  project( "MFCPatcherGUI" )

configuration( { "WIN64" } )
  project( "MFCPatcherGUI" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C" )

configuration( { "WIN32" } )
language( "C++" )

configuration( { "WIN64" } )
language( "C++" )

configuration( { "WIN32" } )


configuration( { WIN32 } )
kind( "SharedLib" )
configuration( { "WIN64" } )


configuration( { WIN64 } )
kind( "SharedLib" )
  
configuration( { WIN32 } )
links( { "gucefCOM", "gucefCOMCORE", "gucefCORE", "gucefMT", "gucefPATCHER", "gucefVFS" } )
  links( { "gucefCOM", "gucefCOMCORE", "gucefCORE", "gucefMT", "gucefPATCHER", "gucefVFS" } )
  

configuration( { WIN32 } )
defines( { "_AFXDLL" } )
  
configuration( { WIN64 } )
links( { "gucefCOM", "gucefCOMCORE", "gucefCORE", "gucefMT", "gucefPATCHER", "gucefVFS" } )
  links( { "gucefCOM", "gucefCOMCORE", "gucefCORE", "gucefMT", "gucefPATCHER", "gucefVFS" } )
  

configuration( { WIN64 } )
defines( { "_AFXDLL" } )


configuration( { "WIN32" } )
    vpaths { ["Platform Headers"] = { "**.h", "**.hpp", "**.hxx" } }
    files( {
      "include/CGUCEFAppWin32MFCDriver.h",
      "include/CMFCCommandLineInfo.h",
      "include/ChildView.h",
      "include/MFCPatcherGUI.h",
      "include/MainFrm.h",
      "include/stdafx.h",
      "projects/MVC8/res/Resource.h",
      "projects/MVC9/res/Resource.h"
    } )

    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "src/CGUCEFAppWin32MFCDriver.cpp",
      "src/CMFCCommandLineInfo.cpp",
      "src/ChildView.cpp",
      "src/MFCPatcherGUI.cpp",
      "src/MainFrm.cpp",
      "src/stdafx.cpp"
    } )



configuration( { "WIN64" } )
    vpaths { ["Platform Headers"] = { "**.h", "**.hpp", "**.hxx" } }
    files( {
      "include/CGUCEFAppWin32MFCDriver.h",
      "include/CMFCCommandLineInfo.h",
      "include/ChildView.h",
      "include/MFCPatcherGUI.h",
      "include/MainFrm.h",
      "include/stdafx.h",
      "projects/MVC8/res/Resource.h",
      "projects/MVC9/res/Resource.h"
    } )

    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "src/CGUCEFAppWin32MFCDriver.cpp",
      "src/CMFCCommandLineInfo.cpp",
      "src/ChildView.cpp",
      "src/MFCPatcherGUI.cpp",
      "src/MainFrm.cpp",
      "src/stdafx.cpp"
    } )


configuration( {} )
includedirs( { "../../../common/include", "../../../platform/gucefCOM/include", "../../../platform/gucefCOMCORE/include", "../../../platform/gucefCORE/include", "../../../platform/gucefMT/include", "../../../platform/gucefPATCHER/include", "../../../platform/gucefVFS/include" } )

configuration( { "WIN32" } )
includedirs( { "../../../platform/gucefCOMCORE/include/mswin", "../../../platform/gucefCORE/include/mswin", "include", "projects/MVC8/res", "projects/MVC9/res" } )

configuration( { "WIN64" } )
includedirs( { "../../../platform/gucefCOMCORE/include/mswin", "../../../platform/gucefCORE/include/mswin", "include", "projects/MVC8/res", "projects/MVC9/res" } )