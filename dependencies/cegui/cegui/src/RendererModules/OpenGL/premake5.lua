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

-- Configuration for module: CEGUI.RendererModule.OpenGL


project( "CEGUI.RendererModule.OpenGL" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C++" )



configuration( {} )
kind( "SharedLib" )

configuration( {} )
links( { "CEGUI", "glew" } )
links( { "CEGUI", "glew" } )


configuration( {} )
defines( { "CEGUIOPENGLRENDERER_EXPORTS" } )


configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "GL3FBOTextureTarget.cpp",
  "GL3GeometryBuffer.cpp",
  "GL3Renderer.cpp",
  "GLFBOTextureTarget.cpp",
  "GLGeometryBuffer.cpp",
  "GLRenderer.cpp",
  "GeometryBufferBase.cpp",
  "RendererBase.cpp",
  "Shader.cpp",
  "ShaderManager.cpp",
  "StateChangeWrapper.cpp",
  "Texture.cpp",
  "TextureTarget.cpp",
  "ViewportTarget.cpp"
 } )



configuration( { "LINUX32" } )
    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "GLXPBTextureTarget.cpp"
    } )



configuration( { "LINUX64" } )
    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "GLXPBTextureTarget.cpp"
    } )



configuration( { "OSX" } )
    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "ApplePBTextureTarget.cpp"
    } )



configuration( { "WIN32" } )
    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "WGLPBTextureTarget.cpp"
    } )



configuration( { "WIN64" } )
    vpaths { ["Platform Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
    files( {
      "WGLPBTextureTarget.cpp"
    } )


configuration( {} )
includedirs( { "../../../../../glew/include", "../../../../../glm", "../../../include" } )