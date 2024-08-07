#-------------------------------------------------------------------
# This file was automatically generated by ProjectGenerator
# which is tooling part the build system designed for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------
#

# Configuration for module: MyGUI.OpenGLPlatform

set( HEADER_FILES 
  ../../../Common/FileSystemInfo/FileSystemInfo.h
  include/MyGUI_OpenGLDataManager.h
  include/MyGUI_OpenGLDiagnostic.h
  include/MyGUI_OpenGLImageLoader.h
  include/MyGUI_OpenGLPlatform.h
  include/MyGUI_OpenGLRTTexture.h
  include/MyGUI_OpenGLRenderManager.h
  include/MyGUI_OpenGLTexture.h
  include/MyGUI_OpenGLVertexBuffer.h
  include/GL/glew.h
  include/GL/glxew.h
  include/GL/wglew.h
)

set( SOURCE_FILES 
  include/GL/glew.c
  src/MyGUI_OpenGLDataManager.cpp
  src/MyGUI_OpenGLPlatform.cpp
  src/MyGUI_OpenGLRTTexture.cpp
  src/MyGUI_OpenGLRenderManager.cpp
  src/MyGUI_OpenGLTexture.cpp
  src/MyGUI_OpenGLVertexBuffer.cpp
)

# Make sure the PLATFORM_HEADER_FILES variable is always defined
 set( PLATFORM_HEADER_FILES "" )

# Make sure the PLATFORM_SOURCE_FILES variable is always defined
 set( PLATFORM_SOURCE_FILES "" )

# For ease of use make a variable that has all files for this module
set( ALL_FILES ${HEADER_FILES} ${SOURCE_FILES} ${PLATFORM_HEADER_FILES} ${PLATFORM_SOURCE_FILES} )

set( MODULE_NAME "MyGUI.OpenGLPlatform" )
add_library( ${MODULE_NAME} STATIC ${ALL_FILES} )
add_dependencies( ${MODULE_NAME} MyGUI.Engine )
target_link_libraries( ${MODULE_NAME} MyGUI.Engine )

if ( ANDROID )
  target_link_libraries( ${MODULE_NAME} GLESv1_CM )
endif( ANDROID )

if ( LINUX32 )
  target_link_libraries( ${MODULE_NAME} GL )
endif( LINUX32 )

if ( LINUX64 )
  target_link_libraries( ${MODULE_NAME} GL )
endif( LINUX64 )

if ( WIN32 )
  target_link_libraries( ${MODULE_NAME} opengl32 )
endif( WIN32 )

if ( WIN64 )
  target_link_libraries( ${MODULE_NAME} opengl32 )
endif( WIN64 )

include_directories( ../../../../freetype/include ../../../../freetype/include/freetype ../../../../freetype/include/freetype/config ../../../../freetype/include/freetype/internal ../../../../freetype/include/freetype/internal/services ../../../../freetype/src ../../../../freetype/src/winfonts ../../../MyGUIEngine/include ../../../Common/FileSystemInfo include include/GL )



#******** START OF AUTOGENERATED TEMPLATED ADDITION CONTENT ********




if (ANDROID)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (LINUX32)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (LINUX64)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (OSX)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (WIN32)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (WIN64)
GUCEF_config_lib( ${MODULE_NAME} )
else()
GUCEF_config_lib( ${MODULE_NAME} )
endif()



#******** END OF AUTOGENERATED TEMPLATED ADDITION CONTENT ********


