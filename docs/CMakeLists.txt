#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

###############################################################
# Install documentation
# Build API docs, if doxygen is available
###############################################################

# Set docs target directory
if (WIN32)
  set(GUCEF_DOCS_PATH "Docs")
elseif (UNIX)
  set(GUCEF_DOCS_PATH "share/GUCEF/docs")
elseif (APPLE)
  # TODO
endif ()
  
if (OGRE_INSTALL_DOCS)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${GUCEF_DOCS_PATH}
    PATTERN ".svn" EXCLUDE
    PATTERN "src" EXCLUDE
  )
endif ()

# Build and install API documentation if doxygen is available
if(DOXYGEN_FOUND)
	configure_file(${GUCEF_TEMPLATES_DIR}/html.cfg.in ${GUCEF_BINARY_DIR}/html.cfg)
	add_custom_target(doc COMMAND 
    ${DOXYGEN_EXECUTABLE} ${GUCEF_BINARY_DIR}/html.cfg 
    WORKING_DIRECTORY ${GUCEF_SOURCE_DIR}/Docs/src/ 
    COMMENT "Building documentation" VERBATIM
  )
	file(WRITE ${GUCEF_BINARY_DIR}/api/temp.txt "This file should be deleted before installation. It is only here to force the creation of the api folder")
	file(REMOVE ${GUCEF_BINARY_DIR}/api/temp.txt)
	install(DIRECTORY ${GUCEF_BINARY_DIR}/api DESTINATION ${GUCEF_DOCS_PATH})
endif()
