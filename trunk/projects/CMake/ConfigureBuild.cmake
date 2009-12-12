#-------------------------------------------------------------------
# This file is part of the CMake build system for GUCEF
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

#######################################################################
# This file takes care of configuring GUCEF to build with the settings
# given in CMake. It creates the necessary config.h file and will 
# also prepare package files for pkg-config and CMake.
#######################################################################

if (GUCEF_BUILD_PLATFORM_IPHONE)
  set(GUCEF_SET_BUILD_PLATFORM_IPHONE 1)
  set(GUCEF_STATIC 1)
  set(GUCEF_STATIC_LIB 1)
endif()

# should we build static libs?
if (GUCEF_STATIC)
  set(GUCEF_LIB_TYPE STATIC)
else ()
  set(GUCEF_LIB_TYPE SHARED)
endif ()

# configure threading options
set(GUCEF_THREAD_PROVIDER 0)
if (GUCEF_CONFIG_THREADS)
	if (UNIX)
		add_definitions(-pthread)
	endif ()

	if (GUCEF_CONFIG_THREAD_PROVIDER STREQUAL "boost")
		set(GUCEF_THREAD_PROVIDER 1)
		include_directories(${Boost_INCLUDE_DIRS})
		# On MSVC Boost usually tries to autolink boost libraries. However since
		# this behaviour is not available on all compilers, we need to find the libraries
		# ourselves, anyway. Disable auto-linking to avoid mess-ups.
		add_definitions(-DBOOST_ALL_NO_LIB)
		set(GUCEF_THREAD_LIBRARIES ${Boost_LIBRARIES})
	endif ()

	if (GUCEF_CONFIG_THREAD_PROVIDER STREQUAL "poco")
		set(GUCEF_THREAD_PROVIDER 2)
		include_directories(${POCO_INCLUDE_DIRS})
		set(GUCEF_THREAD_LIBRARIES ${POCO_LIBRARIES})
	endif ()

	if (GUCEF_CONFIG_THREAD_PROVIDER STREQUAL "tbb")
		set(GUCEF_THREAD_PROVIDER 3)
		include_directories(${TBB_INCLUDE_DIRS})
		set(GUCEF_THREAD_LIBRARIES ${TBB_LIBRARIES})
	endif ()
endif()


# determine config values depending on build options 
set(GUCEF_SET_DOUBLE 0)
set(GUCEF_SET_ALLOCATOR ${GUCEF_CONFIG_ALLOCATOR})
set(GUCEF_SET_CONTAINERS_USE_ALLOCATOR 0)
set(GUCEF_SET_STRING_USE_ALLOCATOR 0)
set(GUCEF_SET_MEMTRACK_DEBUG 0)
set(GUCEF_SET_MEMTRACK_RELEASE 0)
set(GUCEF_SET_THREADS ${GUCEF_CONFIG_THREADS})
set(GUCEF_SET_THREAD_PROVIDER ${GUCEF_THREAD_PROVIDER})
set(GUCEF_SET_DISABLE_FREEIMAGE 0)
set(GUCEF_SET_DISABLE_DDS 0)
set(GUCEF_SET_DISABLE_ZIP 0)
set(GUCEF_SET_NEW_COMPILERS 0)
set(GUCEF_STATIC_LIB 0)
set(GUCEF_SET_USE_BOOST 0)
if (GUCEF_CONFIG_DOUBLE)
  set(GUCEF_SET_DOUBLE 1)
endif()
if (GUCEF_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR)
  set(GUCEF_SET_CONTAINERS_USE_ALLOCATOR 1)
endif ()
if (GUCEF_CONFIG_STRING_USE_CUSTOM_ALLOCATOR)
  set(GUCEF_SET_STRING_USE_ALLOCATOR 1)
endif ()
if (GUCEF_CONFIG_MEMTRACK_DEBUG)
  set(GUCEF_SET_MEMTRACK_DEBUG 1)
endif()
if (GUCEF_CONFIG_MEMTRACK_RELEASE)
  set(GUCEF_SET_MEMTRACK_RELEASE 1)
endif()
if (GUCEF_CONFIG_DISABLE_FREEIMAGE)
  set(GUCEF_SET_DISABLE_FREEIMAGE 1)
endif()
if (GUCEF_CONFIG_DISABLE_DDS)
  set(GUCEF_SET_DISABLE_DDS 1)
endif()
if (GUCEF_CONFIG_DISABLE_ZIP)
  set(GUCEF_SET_DISABLE_ZIP 1)
endif()
if(GUCEF_CONFIG_NEW_COMPILERS)
  set(GUCEF_SET_NEW_COMPILERS 1)
endif()
if (GUCEF_STATIC)
  set(GUCEF_STATIC_LIB 1)
endif()
if (GUCEF_USE_BOOST)
  set(GUCEF_SET_USE_BOOST 1)
endif()
add_definitions(-DHAVE_GUCEF_BUILDSETTINGS_H)

if (GUCEF_TEST_BIG_ENDIAN)
  set(GUCEF_CONFIG_BIG_ENDIAN 1)
else ()
  set(GUCEF_CONFIG_LITTLE_ENDIAN 1)
endif ()

# generate buildsettings.h 
configure_file(${GUCEF_TEMPLATES_DIR}/gucef_buildsettings.h.in ${GUCEF_BINARY_DIR}/include/gucef_buildsettings.h @ONLY)
install(FILES ${GUCEF_BINARY_DIR}/include/gucef_buildsettings.h DESTINATION include/GUCEF)

# Read contents of the GUCEF_Config.h file
file(READ "${GUCEF_SOURCE_DIR}/common/include/gucef_config.h" GUCEF_CONFIG_H)
# add HAVE_GUCEF_BUILDSETTINGS_H preprocessor define
file(WRITE ${GUCEF_BINARY_DIR}/include/gucef_config.h "#define HAVE_GUCEF_BUILDSETTINGS_H\n${GUCEF_CONFIG_H}")
install(FILES ${GUCEF_BINARY_DIR}/include/gucef_config.h DESTINATION include/GUCEF)


# Create the pkg-config package files on Unix systems
if (UNIX)
  set(GUCEF_LIB_SUFFIX "")
  set(GUCEF_PLUGIN_PREFIX "")
  set(GUCEF_PLUGIN_EXT ".so")
  if (GUCEF_STATIC)
    set(GUCEF_LIB_SUFFIX "${GUCEF_LIB_SUFFIX}Static")
    set(GUCEF_PLUGIN_PREFIX "lib")
    set(GUCEF_PLUGIN_EXT ".a")
  endif ()
  string(TOLOWER "${CMAKE_BUILD_TYPE}" GUCEF_BUILD_TYPE)
  if (GUCEF_BUILD_TYPE STREQUAL "debug")
    set(GUCEF_LIB_SUFFIX "${GUCEF_LIB_SUFFIX}_d")
  endif ()

  set(GUCEF_ADDITIONAL_LIBS "")
  set(GUCEF_CFLAGS "")
  set(GUCEF_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
  if (GUCEF_CONFIG_THREADS GREATER 0)
    set(GUCEF_CFLAGS "-pthread")
    set(GUCEF_ADDITIONAL_LIBS "${GUCEF_ADDITIONAL_LIBS} -lpthread")
  endif ()
  if (GUCEF_STATIC)
    if (GUCEF_CONFIG_THREADS)
      set(GUCEF_ADDITIONAL_LIBS "${GUCEF_ADDITIONAL_LIBS} -lboost-thread-mt")
    endif ()
    # there is no pkgconfig file for freeimage, so we need to add that lib manually
    set(GUCEF_ADDITIONAL_LIBS "${GUCEF_ADDITIONAL_LIBS} -lfreeimage")
    configure_file(${GUCEF_TEMPLATES_DIR}/GUCEFStatic.pc.in ${GUCEF_BINARY_DIR}/pkgconfig/GUCEF${GUCEF_LIB_SUFFIX}.pc @ONLY)
  else ()
    configure_file(${GUCEF_TEMPLATES_DIR}/GUCEF.pc.in ${GUCEF_BINARY_DIR}/pkgconfig/GUCEF${GUCEF_LIB_SUFFIX}.pc @ONLY)
  endif ()
  install(FILES ${GUCEF_BINARY_DIR}/pkgconfig/GUCEF${GUCEF_LIB_SUFFIX}.pc DESTINATION lib/pkgconfig)

  # configure additional packages
  
endif ()

if (GUCEF_STANDALONE_BUILD)
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()

if (MSVC)
  # Enable intrinsics on MSVC in debug mode
  # Not actually necessary in release mode since /O2 implies /Oi but can't easily add this per build type?
  add_definitions(/Oi)
endif (MSVC)

### Commented because the FindGUCEF script can currently fill this role better ###
# # Create the CMake package files
# if (WIN32)
#   set(GUCEF_CMAKE_DIR CMake)
# elseif (UNIX)
#   set(GUCEF_CMAKE_DIR lib/cmake)
# elseif (APPLE)
# endif ()
# configure_file(${GUCEF_TEMPLATES_DIR}/GUCEFConfig.cmake.in ${GUCEF_BINARY_DIR}/cmake/GUCEFConfig.cmake @ONLY)
# configure_file(${GUCEF_TEMPLATES_DIR}/GUCEFConfigVersion.cmake.in ${GUCEF_BINARY_DIR}/cmake/GUCEFConfigVersion.cmake @ONLY)
# install(FILES
#   ${GUCEF_BINARY_DIR}/cmake/GUCEFConfig.cmake
#   ${GUCEF_BINARY_DIR}/cmake/GUCEFConfigVersion.cmake
#   DESTINATION ${GUCEF_CMAKE_DIR}
# )
# 
