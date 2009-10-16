#-------------------------------------------------------------------
# This file is part of the CMake build system for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure HDFiller

set(HEADER_FILES 
  include/OgreXMLMeshSerializer.h
  include/OgreXMLPrerequisites.h
  include/OgreXMLSkeletonSerializer.h
  include/tinystr.h
  include/tinyxml.h
)

set(SOURCE_FILES 
  src/main.cpp
  src/OgreXMLMeshSerializer.cpp
  src/OgreXMLSkeletonSerializer.cpp
  src/tinystr.cpp
  src/tinyxml.cpp
  src/tinyxmlerror.cpp
  src/tinyxmlparser.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_definitions(-DTIXML_USE_STL)
add_executable(OgreXMLConverter ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(OgreXMLConverter ${OGRE_LIBRARIES})
ogre_config_tool(OgreXMLConverter)
