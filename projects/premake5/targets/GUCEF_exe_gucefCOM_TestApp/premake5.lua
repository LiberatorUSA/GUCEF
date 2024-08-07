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


workspace( "GUCEF_exe_gucefCOM_TestApp" )

  platforms( { "ALL", "WIN32", "WIN64" } )

  location( "projects\premake5\targets" )

  --
  -- Includes for all modules in the solution:
  --

filter "ALL"
  include( "platform/gucefCOM" )
  include( "platform/gucefCOMCORE" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "tests/gucefCOM_TestApp" )

filter "WIN32"
  include( "platform/gucefCOM" )
  include( "platform/gucefCOMCORE" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "tests/gucefCOM_TestApp" )
  include( "tools/MemoryLeakFinder" )

filter "WIN64"
  include( "platform/gucefCOM" )
  include( "platform/gucefCOMCORE" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "tests/gucefCOM_TestApp" )
  include( "tools/MemoryLeakFinder" )
