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

-- Configuration for module: Direct3D10


configuration( { "WIN32" } )
  project( "Direct3D10" )

configuration( { "WIN64" } )
  project( "Direct3D10" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( { "WIN32" } )
language( "C" )

configuration( { "WIN64" } )
language( "C" )

configuration( { "WIN32" } )
configuration( { "WIN64" } )
  links( { "d3d10", "d3dx10" } )
  links( { "d3d10", "d3dx10" } )

configuration( { "WIN32" } )
includedirs( { "#$#ENVVAR:DXSDK_DIR#$#\Include" } )

configuration( { "WIN64" } )
includedirs( { "#$#ENVVAR:DXSDK_DIR#$#\Include" } )