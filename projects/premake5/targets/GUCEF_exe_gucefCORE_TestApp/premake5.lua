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


workspace( "GUCEF_exe_gucefCORE_TestApp" )

  platforms( { "ALL", "LINUX32", "LINUX64", "WIN32", "WIN64" } )

  location( "projects\premake5\targets" )

  --
  -- Includes for all modules in the solution:
  --

filter "ALL"
  include( "tests/gucefCORE_TestApp" )

filter "LINUX32"
  include( "dependencies/json-builder" )
  include( "dependencies/json-parser" )
  include( "dependencies/libparsifal" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "plugins/CORE/dstorepluginJSONPARSER" )
  include( "plugins/CORE/dstorepluginPARSIFALXML" )
  include( "tests/gucefCORE_TestApp" )

filter "LINUX64"
  include( "dependencies/json-builder" )
  include( "dependencies/json-parser" )
  include( "dependencies/libparsifal" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "plugins/CORE/dstorepluginJSONPARSER" )
  include( "plugins/CORE/dstorepluginPARSIFALXML" )
  include( "tests/gucefCORE_TestApp" )

filter "WIN32"
  include( "dependencies/json-builder" )
  include( "dependencies/json-parser" )
  include( "dependencies/libparsifal" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "plugins/CORE/dstorepluginJSONPARSER" )
  include( "plugins/CORE/dstorepluginPARSIFALXML" )
  include( "tests/gucefCORE_TestApp" )

filter "WIN64"
  include( "dependencies/json-builder" )
  include( "dependencies/json-parser" )
  include( "dependencies/libparsifal" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "plugins/CORE/dstorepluginJSONPARSER" )
  include( "plugins/CORE/dstorepluginPARSIFALXML" )
  include( "tests/gucefCORE_TestApp" )