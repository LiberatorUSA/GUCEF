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
)

set(SOURCE_FILES 
  src/HDFiller.cpp
)

# TODO: the following is an example section, you have to complete it
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_definitions(-DTIXML_USE_STL)
add_executable(HDFiller ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(HDFiller ${OGRE_LIBRARIES})
ogre_config_tool(HDFiller)
