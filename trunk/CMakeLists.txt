#-------------------------------------------------------------------
# This file is part of the CMake build system for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

######################################################################
# GUCEF BUILD SYSTEM
# Welcome to the CMake build system for GUCEF.
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################

cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(GUCEF)

# Include necessary submodules
set(CMAKE_MODULE_PATH 
  "${GUCEF_SOURCE_DIR}/projects/CMake"
  "${GUCEF_SOURCE_DIR}/projects/CMake/Utils" 
  "${GUCEF_SOURCE_DIR}/projects/CMake/Packages"
)
include(CMakeDependentOption)
include(MacroLogFeature)
include(GUCEFConfigTargets)
include(PreprocessorUtils)
set(GUCEF_TEMPLATES_DIR "${GUCEF_SOURCE_DIR}/projects/CMake/Templates")
set(GUCEF_WORK_DIR ${GUCEF_BINARY_DIR})


#####################################################################
# Set up the basic build environment
#####################################################################

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

# determine GUCEF version numbers
include(GUCEFGetVersion)
GUCEF_get_version(${GUCEF_SOURCE_DIR}/common/include/gucef_prerequisites.h)
message(STATUS "Configuring GUCEF ${GUCEF_VERSION}")

if (NOT APPLE)
  # Create debug libraries with _d postfix
  set(CMAKE_DEBUG_POSTFIX "_d")
endif ()

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-msse)
endif ()
if (MSVC)
  add_definitions(/fp:fast)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
  # Test for GCC visibility
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-fvisibility=hidden GUCEF_GCC_VISIBILITY)
  if (GUCEF_GCC_VISIBILITY)
    # determine gcc version
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion 
      OUTPUT_VARIABLE GUCEF_GCC_VERSION)
    message(STATUS "Detected g++ ${GUCEF_GCC_VERSION}")
    message(STATUS "Enabling GCC visibility flags")
    set(GUCEF_GCC_VISIBILITY_FLAGS "-DGUCEF_GCC_VISIBILITY -fvisibility=hidden")
  
    # check if we can safely add -fvisibility-inlines-hidden
    string(TOLOWER "${CMAKE_BUILD_TYPE}" GUCEF_BUILD_TYPE)
    if (GUCEF_BUILD_TYPE STREQUAL "debug" AND GUCEF_GCC_VERSION VERSION_LESS "4.2")
      message(STATUS "Skipping -fvisibility-inlines-hidden due to possible bug in g++ < 4.2")
    else ()
      set(GUCEF_GCC_VISIBILITY_FLAGS "${GUCEF_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
    endif ()
  endif (GUCEF_GCC_VISIBILITY)
endif (CMAKE_COMPILER_IS_GNUCXX)

# determine system endianess
include(TestBigEndian)
test_big_endian(GUCEF_TEST_BIG_ENDIAN)

# Add common include path
include_directories("${GUCEF_SOURCE_DIR}/common/include")
#include_directories("${GUCEF_BINARY_DIR}/include")
if (APPLE)
  include_directories("${CMAKE_SOURCE_DIR}/GUCEFMain/include/OSX")
endif (APPLE)

# Find dependencies
include(Dependencies)
# definitions for samples
set(GUCEF_LIBRARIES gucefCORE ) 


# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${GUCEF_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GUCEF_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GUCEF_BINARY_DIR}/bin")
if (WIN32 OR APPLE)
  if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    # We don't want to install in default system location, install is really for the SDK, so call it that
    set(CMAKE_INSTALL_PREFIX
	  "${GUCEF_BINARY_DIR}/sdk" CACHE PATH "GUCEF install prefix" FORCE
    )
  endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

# Set up iPhone overrides.
if (GUCEF_BUILD_PLATFORM_IPHONE)
  # Force gcc <= 4.2 on iPhone
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(gcc-4.2 GNU)
  CMAKE_FORCE_CXX_COMPILER(gcc-4.2 GNU)

  include_directories("${CMAKE_SOURCE_DIR}/GUCEFMain/include/iPhone")

  # Set build variables
  set(CMAKE_OSX_SYSROOT iphoneos3.0)
  set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_UNIVERSAL_IPHONE_OS)")
  set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit ${GUCEF_SOURCE_DIR}/iPhoneDependencies/lib/release/libboost_date_time.a ${GUCEF_SOURCE_DIR}/iPhoneDependencies/lib/release/libboost_thread.a")
  set(XCODE_ATTRIBUTE_SDKROOT iphoneos3.0)
  set(GUCEF_BUILD_RENDERSYSTEM_GLES TRUE CACHE BOOL "Forcing OpenGL ES RenderSystem for iPhone" FORCE)
  set(GUCEF_STATIC TRUE CACHE BOOL "Forcing static build for iPhone" FORCE)
  set(GUCEF_SET_USE_BOOST TRUE CACHE BOOL "Forcing use of Boost libraries for iPhone" FORCE)
  set(GUCEF_USE_BOOST TRUE CACHE BOOL "Forcing use of Boost libraries for iPhone" FORCE)
  set(Boost_FOUND TRUE CACHE BOOL "Forcing use of Boost libraries for iPhone" FORCE)
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")

  add_definitions(-fno-regmove)
  remove_definitions(-msse)
elseif (APPLE AND NOT GUCEF_BUILD_PLATFORM_IPHONE)
  # Force gcc <= 4.0 on Mac OS X because 4.2 is not supported prior to Mac OS X 10.5
  include(CMakeForceCompiler)
  CMAKE_FORCE_C_COMPILER(gcc-4.0 GNU)
  CMAKE_FORCE_CXX_COMPILER(gcc-4.0 GNU)

  # Set 10.4 as the base SDK by default
  set(XCODE_ATTRIBUTE_SDKROOT macosx10.4)

  # Make sure that the OpenGL render system is selected for non-iPhone Apple builds
  set(GUCEF_BUILD_RENDERSYSTEM_GL TRUE)
  set(GUCEF_BUILD_RENDERSYSTEM_GLES FALSE)
endif ()

######################################################################
# Provide user options to customise the build process
######################################################################

# Customise what to build
option(GUCEF_STATIC "Static build" FALSE)
set(GUCEF_CONFIG_ALLOCATOR 2 CACHE STRING
"Specify the memory allocator to use. Possible values:
  1 - Standard allocator
  2 - nedmalloc
  3 - User-provided allocator"
)
option(GUCEF_CONFIG_MEMTRACK_DEBUG "Enable GUCEF's memory tracker in debug mode" FALSE)
option(GUCEF_CONFIG_MEMTRACK_RELEASE "Enable GUCEF's memory tracker in release mode" FALSE)


###################################################################
# configure global build settings based on selected build options
###################################################################

include(ConfigureBuild)

##################################################################
# Now setup targets
##################################################################

# Setup GUCEF modules
add_subdirectory( gucefMT )
add_subdirectory( gucefCORE )
add_subdirectory( gucefVFS )
add_subdirectory( gucefCOMCORE )
add_subdirectory( gucefGUI )
add_subdirectory( gucefIMAGE )
add_subdirectory( gucefMATH )
add_subdirectory( gucefINPUT )
add_subdirectory( gucefAUDIO )
add_subdirectory( gucefCOM )
add_subdirectory( gucefMULTIPLAY )
add_subdirectory( gucefDRN )
add_subdirectory( gucefPATCHER )

# Setup GUCEF plugin dependencies
add_subdirectory( gucefCORE/plugins/dstorepluginPARSIFALXML/dependancy/libparsifal )
add_subdirectory( gucefINPUT/plugins/inputdriverOIS/dependency/OIS )
add_subdirectory( gucefIMAGE/plugins/imgpluginFreeImage/dependencies/FreeImage )
add_subdirectory( gucefVFS/plugins/vfspluginZIP/dependencies/zlib )
add_subdirectory( gucefVFS/plugins/vfspluginZIP/dependencies/zziplib/zzip )

# Setup GUCEF Plugin modules
add_subdirectory( gucefCORE/plugins/dstorepluginPARSIFALXML )
add_subdirectory( gucefVFS/plugins/vfspluginDVP )
add_subdirectory( gucefVFS/plugins/vfspluginVP )
add_subdirectory( gucefVFS/plugins/vfspluginZIP )
add_subdirectory( gucefIMAGE/plugins/imgpluginFreeImage )
#add_subdirectory( gucefIMAGE/plugins/imgpluginDEVIL )
add_subdirectory( gucefINPUT/plugins/inputdriverOIS )
add_subdirectory( gucefINPUT/plugins/inputdriverMSWINMSG )
add_subdirectory( gucefINPUT/plugins/inputdriverDIRECTINPUT8 )

# Setup GUCEF Test applications
add_subdirectory( gucefVFS_TestApp )
add_subdirectory( gucefCOM_TestApp )
add_subdirectory( gucefCOMCORE_TestApp )
add_subdirectory( gucefMT_TestApp )
add_subdirectory( gucefCORE_TestApp )
#add_subdirectory( gucefGUI_TestApp )
add_subdirectory( gucefIMAGE_TestApp )
#add_subdirectory( gucefMULTIPLAY_TestApp )
#add_subdirectory( gucefMATH_TestApp )
add_subdirectory( gucefINPUT_TestApp )
#add_subdirectory( gucefAUDIO_TestApp )
add_subdirectory( gucefDRN_TestApp )

# Setup GUCEF tooling applications
add_subdirectory(tools)

# Install documentation
add_subdirectory(docs)

# Install CMake modules
#add_subdirectory(CMake)

# Provide CPack packaging target
#include(Packaging)

# Show feature summary
include(FeatureSummary)

