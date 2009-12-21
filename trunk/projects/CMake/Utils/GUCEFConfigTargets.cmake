#-------------------------------------------------------------------
# This file is part of the CMake build system for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure settings and install targets

if (WIN32)
  set(GUCEF_RELEASE_PATH "/Release")
  set(GUCEF_RELWDBG_PATH "/RelWithDebInfo")
  set(GUCEF_MINSIZE_PATH "/MinSizeRel")
  set(GUCEF_DEBUG_PATH "/Debug")
  set(GUCEF_LIB_RELEASE_PATH "/Release")
  set(GUCEF_LIB_RELWDBG_PATH "/RelWithDebInfo")
  set(GUCEF_LIB_MINSIZE_PATH "/MinSizeRel")
  set(GUCEF_LIB_DEBUG_PATH "/Debug")
  set(GUCEF_PLUGIN_PATH "/opt")
elseif (UNIX)
  set(GUCEF_RELEASE_PATH "")
  set(GUCEF_RELWDBG_PATH "")
  set(GUCEF_MINSIZE_PATH "")
  set(GUCEF_DEBUG_PATH "/debug")
  set(GUCEF_LIB_RELEASE_PATH "")
  set(GUCEF_LIB_RELWDBG_PATH "")
  set(GUCEF_LIB_MINSIZE_PATH "")
  set(GUCEF_LIB_DEBUG_PATH "")
  set(GUCEF_PLUGIN_PATH "/GUCEF")
endif ()

# create vcproj.user file for Visual Studio to set debug working directory
function(GUCEF_create_vcproj_userfile TARGETNAME)
  if (MSVC)
    configure_file(
	  ${GUCEF_TEMPLATES_DIR}/VisualStudioUserFile.vcproj.user.in
	  ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
	  @ONLY
	)
  endif ()
endfunction(GUCEF_create_vcproj_userfile)

# install targets according to current build type
function(GUCEF_install_target TARGETNAME SUFFIX)
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${GUCEF_RELEASE_PATH}" CONFIGURATIONS Release None ""
    LIBRARY DESTINATION "lib${GUCEF_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
    ARCHIVE DESTINATION "lib${GUCEF_LIB_RELEASE_PATH}${SUFFIX}" CONFIGURATIONS Release None ""
    FRAMEWORK DESTINATION "bin${GUCEF_RELEASE_PATH}" CONFIGURATIONS Release None ""
  )
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${GUCEF_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
    LIBRARY DESTINATION "lib${GUCEF_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
    ARCHIVE DESTINATION "lib${GUCEF_LIB_RELWDBG_PATH}${SUFFIX}" CONFIGURATIONS RelWithDebInfo
    FRAMEWORK DESTINATION "bin${GUCEF_RELWDBG_PATH}" CONFIGURATIONS RelWithDebInfo
  )
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${GUCEF_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
    LIBRARY DESTINATION "lib${GUCEF_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
    ARCHIVE DESTINATION "lib${GUCEF_LIB_MINSIZE_PATH}${SUFFIX}" CONFIGURATIONS MinSizeRel
    FRAMEWORK DESTINATION "bin${GUCEF_MINSIZE_PATH}" CONFIGURATIONS MinSizeRel
  )
  install(TARGETS ${TARGETNAME}
    RUNTIME DESTINATION "bin${GUCEF_DEBUG_PATH}" CONFIGURATIONS Debug
    LIBRARY DESTINATION "lib${GUCEF_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
    ARCHIVE DESTINATION "lib${GUCEF_LIB_DEBUG_PATH}${SUFFIX}" CONFIGURATIONS Debug
    FRAMEWORK DESTINATION "bin${GUCEF_DEBUG_PATH}" CONFIGURATIONS Debug
  )
endfunction(GUCEF_install_target)

# setup common target settings
function(GUCEF_config_common TARGETNAME)
  set_target_properties(${TARGETNAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${GUCEF_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${GUCEF_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${GUCEF_BINARY_DIR}/bin
  )
  if(GUCEF_BUILD_PLATFORM_IPHONE)
    set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
    set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
    set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
    set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
    set_target_properties(${TARGETNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${GUCEF_SOURCE_DIR}/GUCEFMain/include/GUCEFStableHeaders.h")
  endif(GUCEF_BUILD_PLATFORM_IPHONE)

  GUCEF_create_vcproj_userfile(${TARGETNAME})

  if (MSVC) 
    add_definitions( "/D_CRT_SECURE_NO_WARNINGS /wd4251 /nologo" )
  endif()

endfunction(GUCEF_config_common)

# setup library build
function(GUCEF_config_lib LIBNAME)
  GUCEF_config_common(${LIBNAME})
  if (GUCEF_STATIC)
    # add static prefix, if compiling static version
    set_target_properties(${LIBNAME} PROPERTIES OUTPUT_NAME ${LIBNAME}Static)

    if(GUCEF_BUILD_PLATFORM_IPHONE)
      set_target_properties(${LIBNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
      set_target_properties(${LIBNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
      set_target_properties(${LIBNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
      set_target_properties(${LIBNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${GUCEF_SOURCE_DIR}/GUCEFMain/include/GUCEFStableHeaders.h")
    endif(GUCEF_BUILD_PLATFORM_IPHONE)
  else (GUCEF_STATIC)
    if (CMAKE_COMPILER_IS_GNUCXX)
      # add GCC visibility flags to shared library build
      set_target_properties(${LIBNAME} PROPERTIES COMPILE_FLAGS "${GUCEF_GCC_VISIBILITY_FLAGS}")
	endif (CMAKE_COMPILER_IS_GNUCXX)
	
	# Set some Mac OS X specific framework settings, including installing the headers in subdirs
	if (APPLE AND NOT GUCEF_BUILD_PLATFORM_IPHONE)
      set_target_properties(${LIBNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
      set_target_properties(${LIBNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${GUCEF_SOURCE_DIR}/GUCEFMain/include/GUCEFStableHeaders.h")
      set_target_properties(${LIBNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
      add_custom_command(TARGET ${LIBNAME} POST_BUILD
        COMMAND mkdir ARGS -p ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/GUCEF.framework/Headers/Threading
	    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp ARGS -exclude .DS_Store -exclude CVS -exclude .svn -exclude 'CMakeLists.txt' -resolve-src-symlinks ${GUCEF_SOURCE_DIR}/GUCEFMain/include/Threading/* ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/GUCEF.framework/Headers/Threading/
        COMMAND mkdir ARGS -p ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/GUCEF.framework/Headers/OSX
	    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp ARGS -exclude .DS_Store -exclude CVS -exclude .svn -exclude 'CMakeLists.txt' -resolve-src-symlinks ${GUCEF_SOURCE_DIR}/GUCEFMain/include/OSX/*.h ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/GUCEF.framework/Headers/OSX/
    )
	endif (APPLE AND NOT GUCEF_BUILD_PLATFORM_IPHONE)
  endif (GUCEF_STATIC)
  GUCEF_install_target(${LIBNAME} "")
  
  if (GUCEF_INSTALL_PDB)
    # install debug pdb files
    if (GUCEF_STATIC)
	  install(FILES ${GUCEF_BINARY_DIR}/lib${GUCEF_LIB_DEBUG_PATH}/${LIBNAME}Static_d.pdb
	    DESTINATION lib${GUCEF_LIB_DEBUG_PATH}
		CONFIGURATIONS Debug
	  )
	  install(FILES ${GUCEF_BINARY_DIR}/lib${GUCEF_LIB_RELWDBG_PATH}/${LIBNAME}Static.pdb
	    DESTINATION lib${GUCEF_LIB_RELWDBG_PATH}
		CONFIGURATIONS RelWithDebInfo
	  )
	else ()
	  install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_DEBUG_PATH}/${LIBNAME}_d.pdb
	    DESTINATION bin${GUCEF_DEBUG_PATH}
		CONFIGURATIONS Debug
	  )
	  install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_RELWDBG_PATH}/${LIBNAME}.pdb
	    DESTINATION bin${GUCEF_RELWDBG_PATH}
		CONFIGURATIONS RelWithDebInfo
	  )
	endif ()
  endif ()
endfunction(GUCEF_config_lib)

# setup plugin build
function(GUCEF_config_plugin PLUGINNAME)
  GUCEF_config_common(${PLUGINNAME})
  if (GUCEF_STATIC)
    # add static prefix, if compiling static version
    set_target_properties(${PLUGINNAME} PROPERTIES OUTPUT_NAME ${PLUGINNAME}Static)

    if(GUCEF_BUILD_PLATFORM_IPHONE)
      set_target_properties(${PLUGINNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
      set_target_properties(${PLUGINNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
      set_target_properties(${PLUGINNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
      set_target_properties(${PLUGINNAME} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${GUCEF_SOURCE_DIR}/GUCEFMain/include/GUCEFStableHeaders.h")
    endif(GUCEF_BUILD_PLATFORM_IPHONE)
  else (GUCEF_STATIC)
    if (CMAKE_COMPILER_IS_GNUCXX)
      # add GCC visibility flags to shared library build
      set_target_properties(${PLUGINNAME} PROPERTIES COMPILE_FLAGS "${GUCEF_GCC_VISIBILITY_FLAGS}")
      # disable "lib" prefix on Unix
      set_target_properties(${PLUGINNAME} PROPERTIES PREFIX "")
	endif (CMAKE_COMPILER_IS_GNUCXX)	
  endif (GUCEF_STATIC)
  GUCEF_install_target(${PLUGINNAME} ${GUCEF_PLUGIN_PATH})

  if (GUCEF_INSTALL_PDB)
    # install debug pdb files
    if (GUCEF_STATIC)
	  install(FILES ${GUCEF_BINARY_DIR}/lib${GUCEF_LIB_DEBUG_PATH}/${PLUGINNAME}Static_d.pdb
	    DESTINATION lib${GUCEF_LIB_DEBUG_PATH}/opt
		CONFIGURATIONS Debug
	  )
	  install(FILES ${GUCEF_BINARY_DIR}/lib${GUCEF_LIB_RELWDBG_PATH}/${PLUGINNAME}Static.pdb
	    DESTINATION lib${GUCEF_LIB_RELWDBG_PATH}/opt
		CONFIGURATIONS RelWithDebInfo
	  )
	else ()
	  install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_DEBUG_PATH}/${PLUGINNAME}_d.pdb
	    DESTINATION bin${GUCEF_DEBUG_PATH}
		CONFIGURATIONS Debug
	  )
	  install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_RELWDBG_PATH}/${PLUGINNAME}.pdb
	    DESTINATION bin${GUCEF_RELWDBG_PATH}
		CONFIGURATIONS RelWithDebInfo
	  )
	endif ()
  endif ()
endfunction(GUCEF_config_plugin)

# setup GUCEF demo build
function(GUCEF_config_sample SAMPLENAME)
  # The PRODUCT_NAME target setting cannot contain underscores.  Just remove them
  # Known bug in Xcode CFBundleIdentifier processing rdar://6187020
  # Can cause an instant App Store rejection. Also, code signing will fail. 
  #if (GUCEF_BUILD_PLATFORM_IPHONE)
#    string (REPLACE "_" "" SAMPLENAME ${SAMPLENAME})
  #endif()
  GUCEF_config_common(${SAMPLENAME})

  # set install RPATH for Unix systems
  if (UNIX AND GUCEF_FULL_RPATH)
    set_property(TARGET ${SAMPLENAME} APPEND PROPERTY
      INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
    set_property(TARGET ${SAMPLENAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif ()
  
  if (APPLE)
    # On OS X, create .app bundle
    set_property(TARGET ${SAMPLENAME} PROPERTY MACOSX_BUNDLE TRUE)

	if (GUCEF_BUILD_PLATFORM_IPHONE)
      set (GUCEF_SAMPLE_CONTENTS_PATH 
        ${CMAKE_BINARY_DIR}/bin/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)/$(PRODUCT_NAME).app)
      add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND cp ARGS ${CMAKE_BINARY_DIR}/bin/*.cfg ${GUCEF_SAMPLE_CONTENTS_PATH}/
	    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -exclude 'CMakeLists.txt' -resolve-src-symlinks ${GUCEF_SOURCE_DIR}/Samples/Media ${GUCEF_SAMPLE_CONTENTS_PATH}/
	    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -exclude 'CMakeLists.txt' -resolve-src-symlinks ${GUCEF_SOURCE_DIR}/Samples/Common/misc/Icon.png ${GUCEF_SAMPLE_CONTENTS_PATH}/
	    COMMAND /Developer/Library/PrivateFrameworks/DevToolsCore.framework/Resources/pbxcp -exclude .DS_Store -exclude CVS -exclude .svn -exclude 'CMakeLists.txt' -resolve-src-symlinks ${GUCEF_SOURCE_DIR}/Samples/Common/misc/Default.png ${GUCEF_SAMPLE_CONTENTS_PATH}/
      )
	else ()
      # also, symlink frameworks so .app is standalone
      # NOTE: $(CONFIGURATION) is not resolvable at CMake run time, it's only 
      # valid at build time (hence parenthesis rather than braces)
      set (GUCEF_SAMPLE_CONTENTS_PATH 
        ${CMAKE_BINARY_DIR}/bin/$(CONFIGURATION)/${SAMPLENAME}.app/Contents)
      add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND mkdir ARGS -p ${GUCEF_SAMPLE_CONTENTS_PATH}/Frameworks
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/GUCEF.framework 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Frameworks/
        COMMAND ln ARGS -s -f ${CMAKE_SOURCE_DIR}/Dependencies/Cg.framework 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Frameworks/
        COMMAND ln ARGS -s -f ${CMAKE_SOURCE_DIR}/Dependencies/CEGUI.framework 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Frameworks/
      )
      # now cfg files
      add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND mkdir ARGS -p ${GUCEF_SAMPLE_CONTENTS_PATH}/Resources
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/bin/plugins.cfg 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Resources/
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/bin/resources.cfg 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Resources/
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/bin/media.cfg 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Resources/
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/bin/quake3settings.cfg 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Resources/
      )
      # now plugins
      add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND mkdir ARGS -p ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins)
      if (GUCEF_BUILD_RENDERSYSTEM_GL)
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
          COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/RenderSystem_GL.dylib 
            ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
        )
      endif ()
      if (GUCEF_BUILD_RENDERSYSTEM_GLES)
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
          COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/RenderSystem_GLES.a
            ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
        )
      endif ()
      if (GUCEF_BUILD_PLUGIN_BSP)    
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/Plugin_BSPSceneManager.dylib 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
        )
      endif()
      if (GUCEF_BUILD_PLUGIN_CG)    
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/Plugin_CgProgramManager.dylib 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
        )
      endif()
      if (GUCEF_BUILD_PLUGIN_OCTREE)    
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/Plugin_OctreeSceneManager.dylib 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
       )
      endif()
      if (GUCEF_BUILD_PLUGIN_PCZ)    
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
          COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/Plugin_PCZSceneManager.dylib 
            ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/    
        )
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/Plugin_OctreeZone.dylib 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
      )
      endif()
      if (GUCEF_BUILD_PLUGIN_PFX)    
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/Plugin_ParticleFX.dylib 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
        )
      endif()
      if (GUCEF_BUILD_CEGUIRENDERER)    
        add_custom_command(TARGET ${SAMPLENAME} POST_BUILD
        COMMAND ln ARGS -s -f ${CMAKE_BINARY_DIR}/lib/$(CONFIGURATION)/libCEGUIGUCEFRenderer.dylib 
          ${GUCEF_SAMPLE_CONTENTS_PATH}/Plugins/
        )
      endif()
    endif()
  endif (APPLE)

  if (GUCEF_INSTALL_SAMPLES)
    GUCEF_install_target(${SAMPLENAME} "")
    if (GUCEF_INSTALL_PDB)
      # install debug pdb files
      install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_DEBUG_PATH}/${SAMPLENAME}.pdb
        DESTINATION bin${GUCEF_DEBUG_PATH} CONFIGURATIONS Debug
        )
      install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_RELWDBG_PATH}/${SAMPLENAME}.pdb
        DESTINATION bin${GUCEF_RELWDBG_PATH} CONFIGURATIONS RelWithDebInfo
        )
    endif ()
  endif ()	

endfunction(GUCEF_config_sample)

# setup GUCEF tool build
function(GUCEF_config_tool TOOLNAME)
  GUCEF_config_common(${TOOLNAME})

  # set install RPATH for Unix systems
  if (UNIX AND GUCEF_FULL_RPATH)
    set_property(TARGET ${TOOLNAME} APPEND PROPERTY
      INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
    set_property(TARGET ${TOOLNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif ()

  if (GUCEF_INSTALL_TOOLS)
    GUCEF_install_target(${TOOLNAME} "")
    if (GUCEF_INSTALL_PDB)
      # install debug pdb files
      install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_DEBUG_PATH}/${TOOLNAME}.pdb
        DESTINATION bin${GUCEF_DEBUG_PATH}
        CONFIGURATIONS Debug
        )
      install(FILES ${GUCEF_BINARY_DIR}/bin${GUCEF_RELWDBG_PATH}/${TOOLNAME}.pdb
        DESTINATION bin${GUCEF_RELWDBG_PATH}
        CONFIGURATIONS RelWithDebInfo
        )
    endif ()
  endif ()	

endfunction(GUCEF_config_tool)
