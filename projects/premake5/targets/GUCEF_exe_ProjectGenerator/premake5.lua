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


workspace( "GUCEF_exe_ProjectGenerator" )

  platforms( { "ALL" } )

  location( "projects\premake5\targets" )

  --
  -- Includes for all modules in the solution:
  --

filter "ALL"
  include( "dependencies/libparsifal" )
  include( "platform/gucefCORE" )
  include( "platform/gucefMT" )
  include( "plugins/CORE/dstorepluginPARSIFALXML" )
  include( "tools/ProjectGen" )
  include( "tools/ProjectGen/plugins/ProjectGenDependsFilter" )
  include( "tools/ProjectGen/plugins/ProjectGenVSImporter" )
  include( "tools/ProjectGenerator" )
