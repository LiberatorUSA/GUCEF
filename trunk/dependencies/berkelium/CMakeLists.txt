# Berkelium Build Script
#
# Parameters:
#  BERKELIUM_CHROME_ROOT_SEARCH_PATHS - list of paths to search for berkelium in.  Should be absolute paths.
#  BERKELIUM_WITH_TESTS - if true, build tests. True by default.
#  BERKELIUM_INSTALL_BINARY_PATH - offset for installing binaries, e.g. bin
#  BERKELIUM_INSTALL_LIBRARY_PATH - offset for installing libraries, e.g. lib
#
# Outputs:
#  BERKELIUM_CHROME_FOUND - TRUE if Chrome is found and berkelium will be built.

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(BERKELIUM_WITH_TESTS TRUE CACHE BOOL "If true, build test targets")

IF(APPLE)
    IF(IS_DIRECTORY /Developer/SDKs/MacOSX10.5.sdk)
        SET(MAC_SDK_DIR /Developer/SDKs/MacOSX10.5.sdk)
        SET(MAC_MIN_VER 10.5)
        SET(MAC_EXTRA_FLAGS)
        SET(CMAKE_CXX_COMPILER g++-4.0)
        SET(CMAKE_C_COMPILER gcc-4.0)
    ELSE()
        SET(MAC_SDK_DIR /Developer/SDKs/MacOSX10.6.sdk)
        SET(MAC_MIN_VER 10.6)
        SET(MAC_EXTRA_FLAGS -m32)
        SET(CMAKE_CXX_COMPILER g++-4.2)
        SET(CMAKE_C_COMPILER gcc-4.2)
    ENDIF()
ENDIF()

SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
IF(COMMAND CMAKE_POLICY)
  # Use new policy for absolute path libraries.  We have no reason to depend on the
  # old version which adds library search paths unnecessarily
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

SET(BERKELIUM_TOP_LEVEL ${CMAKE_CURRENT_SOURCE_DIR})
GET_FILENAME_COMPONENT(BERKELIUM_TOP_LEVEL ${BERKELIUM_TOP_LEVEL} ABSOLUTE)

SET(CMAKE_MODULE_PATH
  ${BERKELIUM_TOP_LEVEL}
  ${BERKELIUM_TOP_LEVEL}/build/modules/
)

INCLUDE(ListUtil)
IF(BERKELIUM_WITH_TESTS)
  INCLUDE(CXXTest)
ENDIF()
FIND_PACKAGE(PythonInterp)

IF(NOT DEFINED BERKELIUM_BUILD_DEMOS)
  SET(BERKELIUM_BUILD_DEMOS TRUE CACHE BOOL "If enabled, builds demos if their dependencies are available.")
ELSE()
  SET(BERKELIUM_BUILD_DEMOS BERKELIUM_BUILD_DEMOS CACHE BOOL "If enabled, builds demos if their dependencies are available.")
ENDIF()

IF(NOT CHROME_ROOT)
  SET(CHROME_ROOT_SEARCH_PATHS
    ${BERKELIUM_CHROME_ROOT_SEARCH_PATHS} # User provided search paths go first
    # Followed by a few reasonable choices within this directory
    ${BERKELIUM_TOP_LEVEL}/chrome
    ${BERKELIUM_TOP_LEVEL}/chromium/chromium
    ${BERKELIUM_TOP_LEVEL}/chromium
    ${BERKELIUM_TOP_LEVEL}/build/chromium
    ${BERKELIUM_TOP_LEVEL}/build/c
    )
  FOREACH(CHROME_ROOT_SEARCH_PATH ${CHROME_ROOT_SEARCH_PATHS})
    IF(EXISTS ${CHROME_ROOT_SEARCH_PATH})
      IF(NOT CHROME_ROOT)
        SET(CHROME_ROOT ${CHROME_ROOT_SEARCH_PATH})
      ENDIF()
    ENDIF()
  ENDFOREACH()
ENDIF()
SET(CHROME_FOUND_QUIETLY TRUE)

FIND_PACKAGE(Chrome)

SET(BERKELIUM_CHROME_FOUND ${CHROME_FOUND})
SET(CXXTESTRoot ${BERKELIUM_TOP_LEVEL}/externals/cxxtest)

IF(CHROME_FOUND)
  INCLUDE_DIRECTORIES(${BERKELIUM_TOP_LEVEL}/include ${CHROME_INCLUDE_DIRS} ${CXXTESTRoot})

  GET_FILENAME_COMPONENT(RELATIVE_LIB_LINK_DIR ../lib ABSOLUTE)
  GET_FILENAME_COMPONENT(RELATIVE_CURRENT_LINK_DIR . ABSOLUTE)
  LINK_DIRECTORIES(${CHROME_LIBRARY_DIRS} ${RELATIVE_LIB_LINK_DIR} ${RELATIVE_CURRENT_LINK_DIR})
  SET(BERKELIUM_SOURCE_NAMES src/Berkelium src/Context src/Cursor src/ContextImpl src/ForkedProcessHook src/NavigationController src/RenderWidget src/MemoryRenderViewHost src/Root src/ScriptUtilImpl src/ScriptVariant src/StringUtil src/Window src/WindowImpl src/DevToolsHttpProtocolHandler)

  SET(BERKELIUM_SOURCES)
  FOREACH(BERKELIUM_SOURCE_FILE ${BERKELIUM_SOURCE_NAMES})
    SET(BERKELIUM_SOURCES ${BERKELIUM_SOURCES}   ${BERKELIUM_TOP_LEVEL}/${BERKELIUM_SOURCE_FILE}.cpp)
  ENDFOREACH()

  SET(CHROMIUM_LDFLAGS "")
  FOREACH(CHROME_LDFLAG ${CHROME_LDFLAGS})
    SET(CHROMIUM_LDFLAGS "${CHROMIUM_LDFLAGS} ${CHROME_LDFLAG}")
  ENDFOREACH()
  FOREACH(CHROME_CFLAG ${CHROME_CFLAGS})
    SET(CHROMIUM_CFLAGS "${CHROMIUM_CFLAGS} ${CHROME_CFLAG}")
  ENDFOREACH()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
  IF(APPLE)
    ADD_DEFINITIONS(-isysroot ${MAC_SDK_DIR} -arch i386 ${MAC_EXTRA_FLAGS} -mmacosx-version-min=${MAC_MIN_VER} )
    SET(BERKELIUM_LDFLAGS "-Wl,-syslibroot -Wl,${MAC_SDK} ${MAC_EXTRA_FLAGS}")
  ENDIF()

  IF (NOT APPLE)
    ADD_CHROME_SYMLINK_TARGET()
  ENDIF()

  # libberkelium -- core berkelium library
  ADD_LIBRARY(libberkelium SHARED ${BERKELIUM_SOURCES})
  SET_TARGET_PROPERTIES(libberkelium PROPERTIES DEBUG_POSTFIX "_d")
  SET_TARGET_PROPERTIES(libberkelium PROPERTIES LINK_FLAGS "${CHROMIUM_LDFLAGS} ${BERKELIUM_LDFLAGS}")
  SET_TARGET_PROPERTIES(libberkelium PROPERTIES COMPILE_FLAGS "${CHROMIUM_CFLAGS}")
  SET_TARGET_PROPERTIES(libberkelium PROPERTIES DEBUG_POSTFIX "_d")
  SET_TARGET_PROPERTIES(libberkelium PROPERTIES SKIP_BUILD_RPATH ON)
  TARGET_LINK_LIBRARIES(libberkelium ${CHROME_LIBRARIES})

  IF(APPLE)
    SET( CHROMIUM_DYLIB_EXT .dylib)
  ELSE()
    SET( CHROMIUM_DYLIB_EXT .so)
  ENDIF()

  # berkelium -- renderer binary
  ADD_EXECUTABLE(berkelium ${BERKELIUM_TOP_LEVEL}/subprocess.cpp)
  SET_TARGET_PROPERTIES(berkelium PROPERTIES LINK_FLAGS "${BERKELIUM_LDFLAGS} -Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}")
  TARGET_LINK_LIBRARIES(berkelium -L. debug liblibberkelium_d${CHROMIUM_DYLIB_EXT} optimized liblibberkelium${CHROMIUM_DYLIB_EXT} )
  SET_TARGET_PROPERTIES(berkelium PROPERTIES SKIP_BUILD_RPATH ON)
  ADD_DEPENDENCIES(berkelium libberkelium)

  # Useful values for setting up demo apps
  SET(BERKELIUM_LINK_LIBS -L. debug liblibberkelium_d${CHROMIUM_DYLIB_EXT} optimized liblibberkelium${CHROMIUM_DYLIB_EXT} )

  IF(APPLE)
    ADD_EXECUTABLE(bin_replace ${BERKELIUM_TOP_LEVEL}/util/bin_replace.cpp)
    SET_TARGET_PROPERTIES(bin_replace PROPERTIES LINK_FLAGS "${BERKELIUM_LDFLAGS}")
  ENDIF()

  # ppmrender -- demo app
  IF(BERKELIUM_BUILD_DEMOS)
    ADD_EXECUTABLE(ppmrender ${BERKELIUM_TOP_LEVEL}/demo/ppmrender/ppmmain.cpp)
    TARGET_LINK_LIBRARIES(ppmrender ${BERKELIUM_LINK_LIBS})
    SET_TARGET_PROPERTIES(ppmrender PROPERTIES LINK_FLAGS "${BERKELIUM_LDFLAGS} -Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(ppmrender PROPERTIES SKIP_BUILD_RPATH ON)
    ADD_DEPENDENCIES(ppmrender libberkelium)
    IF(APPLE)
      ADD_CHROME_APP(
        APP ppmrender
        DEPENDS ppmrender plugin_carbon_interpose
        COPY
        ${CMAKE_CURRENT_BINARY_DIR}/berkelium
        )
    ENDIF()
  ENDIF()

  # demo directory, so we can share some implementation between demos
  SET(DEMO_DIR ${BERKELIUM_TOP_LEVEL}/demo)

  # GL Demos
  FIND_PACKAGE(OpenGL)
  FIND_PACKAGE(GLUT)
  IF(OPENGL_FOUND AND GLUT_FOUND)
    # 2.4 Compatibility requires global include directory instead of target specific
    INCLUDE_DIRECTORIES(AFTER ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR} ${DEMO_DIR})
  ENDIF()

  # glut_demo -- demo app
  IF(BERKELIUM_BUILD_DEMOS AND OPENGL_FOUND AND GLUT_FOUND)
    ADD_EXECUTABLE(glut_demo ${BERKELIUM_TOP_LEVEL}/demo/glut_demo/glut_demo.cpp)
    TARGET_LINK_LIBRARIES(glut_demo ${BERKELIUM_LINK_LIBS})
    SET_TARGET_PROPERTIES(glut_demo PROPERTIES LINK_FLAGS "${BERKELIUM_LDFLAGS} -Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}")
    TARGET_LINK_LIBRARIES(glut_demo ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
    SET_TARGET_PROPERTIES(glut_demo PROPERTIES SKIP_BUILD_RPATH ON)
    ADD_DEPENDENCIES(glut_demo libberkelium)
    IF(APPLE)
      ADD_CHROME_APP(
        APP glut_demo
        DEPENDS glut_demo plugin_carbon_interpose
        COPY
        ${CMAKE_CURRENT_BINARY_DIR}/berkelium
        )
    ENDIF()
  ENDIF()

  # glut_input -- glut demo app which also handles input
  IF(BERKELIUM_BUILD_DEMOS AND OPENGL_FOUND AND GLUT_FOUND)
    ADD_EXECUTABLE(glut_input ${BERKELIUM_TOP_LEVEL}/demo/glut_input/glut_input.cpp)
    TARGET_LINK_LIBRARIES(glut_input ${BERKELIUM_LINK_LIBS})
    SET_TARGET_PROPERTIES(glut_input PROPERTIES LINK_FLAGS "${BERKELIUM_LDFLAGS} -Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}")
    TARGET_LINK_LIBRARIES(glut_input ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
    SET_TARGET_PROPERTIES(glut_input PROPERTIES SKIP_BUILD_RPATH ON)
    ADD_DEPENDENCIES(glut_input libberkelium)
    IF(APPLE)
      ADD_CHROME_APP(
        APP glut_input
        DEPENDS glut_input plugin_carbon_interpose
        COPY
        ${CMAKE_CURRENT_BINARY_DIR}/berkelium
        )
    ENDIF()
  ENDIF()

  IF(APPLE)
    ADD_LIBRARY(plugin_carbon_interpose SHARED ${CHROME_ROOT}/src/chrome/browser/plugin_carbon_interpose_mac.cc)
    SET_TARGET_PROPERTIES(plugin_carbon_interpose PROPERTIES COMPILE_FLAGS ${CHROMIUM_CFLAGS})
    SET_TARGET_PROPERTIES(plugin_carbon_interpose PROPERTIES LINK_FLAGS "-framework Carbon ${BERKELIUM_LDFLAGS} -Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}")
    TARGET_LINK_LIBRARIES(plugin_carbon_interpose -L. debug liblibberkelium_d.dylib optimized liblibberkelium.dylib )
    SET_TARGET_PROPERTIES(plugin_carbon_interpose PROPERTIES SKIP_BUILD_RPATH ON)
    ADD_DEPENDENCIES(plugin_carbon_interpose libberkelium)

  ENDIF()

  SET(BERKELIUM_INSTALL_BINARY_PATH bin CACHE STRING "Offset from install prefix to install binaries to")
  SET(BERKELIUM_INSTALL_LIBRARY_PATH lib CACHE STRING "Offset from install prefix to install libraries to")
  INSTALL(TARGETS
          libberkelium
          berkelium
          RUNTIME
          DESTINATION ${BERKELIUM_INSTALL_BINARY_PATH}
          LIBRARY
          DESTINATION ${BERKELIUM_INSTALL_LIBRARY_PATH})
  INSTALL(DIRECTORY include/berkelium
      DESTINATION include)
  IF(NOT APPLE)
    INSTALL(FILES
          ${CHROMIUM_DATADIR}/chrome.pak
          ${CHROMIUM_DATADIR}/resources.pak
            DESTINATION ${BERKELIUM_INSTALL_BINARY_PATH})
    INSTALL(DIRECTORY ${CHROMIUM_DATADIR}/locales
        DESTINATION ${BERKELIUM_INSTALL_BINARY_PATH})
    INSTALL(DIRECTORY ${CHROMIUM_DATADIR}/resources
        DESTINATION ${BERKELIUM_INSTALL_BINARY_PATH})
  ENDIF()

  IF(BERKELIUM_WITH_TESTS)
    SET(TEST_BINARY tests)
    
    #test source files
    SET(TEST_FILES
      ${BERKELIUM_TOP_LEVEL}/test/BlankTest.hpp
      )
    ADD_CXXTEST_CPP_TARGET(
      ${TEST_BINARY}
      LIBRARYDIR ${CXXTESTRoot}
      TESTS ${TEST_FILES}
      RUNNER ${BERKELIUM_TOP_LEVEL}/test/Test.cpp
      DEPENDS berkelium libberkelium)

    TARGET_LINK_LIBRARIES(${TEST_BINARY} ${BERKELIUM_LINK_LIBS})
    SET_TARGET_PROPERTIES(${TEST_BINARY} PROPERTIES LINK_FLAGS "${BERKELIUM_LDFLAGS} -Wl,-rpath,${CMAKE_CURRENT_BINARY_DIR}")
    SET_TARGET_PROPERTIES(${TEST_BINARY} PROPERTIES SKIP_BUILD_RPATH ON)
    ADD_DEPENDENCIES(${TEST_BINARY} berkelium libberkelium)

    ADD_CUSTOM_TARGET(test
      DEPENDS ${TEST_BINARY}
      COMMAND ${TEST_BINARY} $ENV{SINGLE_SUITE})
  ENDIF()
ENDIF(CHROME_FOUND)

FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
  ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} "doc/Doxyfile")
ENDIF()
