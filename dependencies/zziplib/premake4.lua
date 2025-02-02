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

-- Configuration for module: zziplib


project( "zziplib" )

configuration( {} )
  location( os.getenv( "PM4OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM4TARGETDIR" ) )

configuration( {} )
language( "C" )



configuration( {} )
kind( "SharedLib" )


configuration( {} )
defines( { "ZZIP_DLL" } )
  links( { "z" } )
  

configuration( { ANDROID } )
defines( { "ZZIP_HAVE_STDINT_H" } )
  
configuration( { LINUX32 } )
links( { "zlib" } )
  links( { "zlib" } )
  

configuration( { LINUX32 } )
defines( { "ZZIP_HAVE_STDINT_H" } )
  
configuration( { LINUX64 } )
links( { "zlib" } )
  links( { "zlib" } )
  

configuration( { LINUX64 } )
defines( { "ZZIP_HAVE_STDINT_H" } )
  
configuration( { WIN32 } )
links( { "zlib" } )
  links( { "zlib" } )
  
configuration( { WIN64 } )
links( { "zlib" } )
  links( { "zlib" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "dummy.h",
  "zzip/__debug.h",
  "zzip/__dirent.h",
  "zzip/__fnmatch.h",
  "zzip/__hints.h",
  "zzip/__mmap.h",
  "zzip/_config.h",
  "zzip/_msvc.h",
  "zzip/autoconf.h",
  "zzip/conf.h",
  "zzip/fetch.h",
  "zzip/file.h",
  "zzip/format.h",
  "zzip/fseeko.h",
  "zzip/info.h",
  "zzip/lib.h",
  "zzip/memdisk.h",
  "zzip/mmapped.h",
  "zzip/plugin.h",
  "zzip/write.h",
  "zzip/zzip.h",
  "zzip/zzip_stdint.h",
  "zzip/zzip_types.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "zzip/dir.c",
  "zzip/err.c",
  "zzip/fetch.c",
  "zzip/file.c",
  "zzip/fseeko.c",
  "zzip/info.c",
  "zzip/memdisk.c",
  "zzip/mmapped.c",
  "zzip/plugin.c",
  "zzip/stat.c",
  "zzip/write.c",
  "zzip/zip.c"
 } )


configuration( {} )
includedirs( { "../zziplib", "zzip" } )

configuration( { "LINUX32" } )
includedirs( { "../zlib" } )

configuration( { "LINUX64" } )
includedirs( { "../zlib" } )

configuration( { "WIN32" } )
includedirs( { "../zlib" } )

configuration( { "WIN64" } )
includedirs( { "../zlib" } )
