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

-- Configuration for module: lcms2


project( "lcms2" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C" )



configuration( {} )
kind( "SharedLib" )


configuration( {} )
defines( { "CMS_DLL_BUILD" } )
  

configuration( { WIN32 } )
defines( { "CMS_RELY_ON_WINDOWS_STATIC_MUTEX_INIT" } )
  

configuration( { WIN64 } )
defines( { "CMS_RELY_ON_WINDOWS_STATIC_MUTEX_INIT" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "include/lcms.h",
  "include/lcms2.h",
  "include/lcms2_plugin.h",
  "src/lcms2_internal.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src/cmsalpha.c",
  "src/cmscam02.c",
  "src/cmscgats.c",
  "src/cmscnvrt.c",
  "src/cmserr.c",
  "src/cmsgamma.c",
  "src/cmsgmt.c",
  "src/cmshalf.c",
  "src/cmsintrp.c",
  "src/cmsio0.c",
  "src/cmsio1.c",
  "src/cmslut.c",
  "src/cmsmd5.c",
  "src/cmsmtrx.c",
  "src/cmsnamed.c",
  "src/cmsopt.c",
  "src/cmspack.c",
  "src/cmspcs.c",
  "src/cmsplugin.c",
  "src/cmsps2.c",
  "src/cmssamp.c",
  "src/cmssm.c",
  "src/cmstypes.c",
  "src/cmsvirt.c",
  "src/cmswtpnt.c",
  "src/cmsxform.c"
 } )


configuration( {} )
includedirs( { "include", "src" } )