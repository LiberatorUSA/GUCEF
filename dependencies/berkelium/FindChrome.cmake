# FindChrome.cmake
# Searches for Chrome and sets up a few helpful macros.
#
# Outputs:
#  CHROME_FOUND - True if Chrome was found
#  CHROME_INCLUDE_DIRS - List of include directories for Chrome headers
#  CHROME_CFLAGS - C flags to pass to compiler for Chrome
#  CHROME_LIBRARY_DIRS - List of library directories for Chrome
#  CHROME_LDFLAGS - flags to pass to the linker for Chrome
#
# Output Macros:
#  On Mac:
#
#  ADD_CHROME_APP - Adds a Chrome .app target for Mac by generating the
#                   appropriate app structure and symlinking in another target
#                   as the actual executable
#   Paramters:
#    APP - the name of the app to link to, must be a file or existing target
#    LINKS - additional binaries to link into the .app; this almost certainly
#            includes the renderer binary
#    COPY - additional files to copy into the .app
#
#  On Linux:
#  ADD_CHROME_SYMLINK_TARGET - Adds a target which adds symlinks to the
#                              to the necessary Chrome libraries and resources
#   Parameters: None
#

INCLUDE(FindPkgConfig)
INCLUDE(ListUtil)
INCLUDE(ParseArguments)

SET(CHROME_FOUND TRUE)
SET(CHROMIUMDIR ${CHROME_ROOT})
IF(NOT CHROME_MODE)
  SET(CHROMIUMMODE Release)
ELSE()
  SET(CHROMIUMMODE ${CHROME_MODE})
ENDIF()
STRING(COMPARE EQUAL ${CHROMIUMMODE} "Debug" CHROMIUM_ISDEBUG)
IF(CHROMIUM_ISDEBUG)
SET(CHROMIUM_DEBUGFLAGS -D_DEBUG)
ELSE()
SET(CHROMIUM_DEBUGFLAGS -DNDEBUG)
ENDIF()

SET(LINUX_ENABLE_NACL FALSE)

IF(APPLE)
  SET(AS_NEEDED_FLAG)
ELSE()
  SET(AS_NEEDED_FLAG -Wl,--no-as-needed)
ENDIF()

IF(APPLE)
IF(NOT CHROMIUMBUILDER)
SET(CHROMIUMBUILDER "xcode")
ENDIF()
ENDIF()

IF(CHROMIUMBUILDER)
  STRING(COMPARE EQUAL ${CHROMIUMBUILDER} "xcode" CHROMIUM_ISXCODE)
ENDIF()
IF(CHROMIUMBUILDER)
  STRING(COMPARE EQUAL ${CHROMIUMBUILDER} "scons" CHROMIUM_ISSCONS)
ENDIF()
IF(CHROMIUM_ISXCODE)
  SET(CHROMIUM_PLAT mac)
  SET(CHROMIUM_PLAT_CFLAGS -pthread)
  SET(SNOW)

  SET(CHROMIUM_PLAT_LDFLAGS -dynamiclib -pthread "-framework CoreAudio" "-framework AudioToolbox" "-framework CoreFoundation" "-framework Foundation" "-framework AddressBook" "-framework AppKit" "-framework IOKit" "-framework QuartzCore" "-framework Security" "-framework SecurityInterface" "-framework SystemConfiguration" -ObjC "-framework Carbon" "-framework OpenGL" "-framework Accelerate" "-framework AudioUnit")
  SET(CHROMIUM_START_GROUP)
  SET(CHROMIUM_END_GROUP)
  SET(CHROMIUM_DLLEXT dylib)
ELSE()
  PKG_CHECK_MODULES(CHROMIUM_PLAT gtk+-2.0 dbus-glib-1 glib-2.0 gthread-2.0 gio-unix-2.0)  # will set MONO_FOUND
  SET(CHROMIUM_PLAT linux)
  IF(NOT LINUX_ENABLE_NACL)
    SET(CHROMIUM_PLAT_CFLAGS ${CHROMIUM_PLAT_CFLAGS} -DDISABLE_NACL)
  ENDIF()

  SET(CHROMIUM_PLAT_LDFLAGS ${CHROMIUM_PLAT_LDFLAGS}  ${AS_NEEDED_FLAG} smime3 plds4 plc4 pthread gdk-x11-2.0 gdk_pixbuf-2.0 pangocairo-1.0 gio-2.0 pango-1.0 cairo gobject-2.0 gmodule-2.0 glib-2.0 fontconfig freetype rt gconf-2 glib-2.0 dbus-glib-1 X11 Xtst asound harfbuzz sandbox)

  SET(CHROMIUM_START_GROUP -Wl,--start-group)
  SET(CHROMIUM_END_GROUP -Wl,--end-group)
  SET(CHROMIUM_DLLEXT so)
ENDIF()

IF(NOT CHROMIUM_ISSCONS)
  IF(NOT CHROMIUM_ISXCODE)
    ### Make build (Linux 32/64-bit) ###
    SET(CHLIBS $(CHROMIUMLIBPATH))
    SET(CHROMIUM_DATADIR ${CHROMIUMDIR}/src/out/${CHROMIUMMODE})
    IF(NOT CHROMIUM_CHLIBS)
      SET(CHROMIUM_CHLIBS ${CHROMIUM_DATADIR}/obj.target)
    ENDIF()

   SET(CHROME_LIBRARY_DIRS ${CHROMIUM_CHLIBS} ${CHROMIUM_CHLIBS}/app ${CHROMIUM_CHLIBS}/base ${CHROMIUM_CHLIBS}/base/allocator ${CHROMIUM_CHLIBS}/ipc ${CHROMIUM_CHLIBS}/jingle ${CHROMIUM_CHLIBS}/chrome ${CHROMIUM_CHLIBS}/chrome/default_plugin ${CHROMIUM_CHLIBS}/net ${CHROMIUM_CHLIBS}/media ${CHROMIUM_CHLIBS}/webkit ${CHROMIUM_CHLIBS}/sandbox ${CHROMIUM_CHLIBS}/skia ${CHROMIUM_CHLIBS}/remoting ${CHROMIUM_CHLIBS}/remoting/proto ${CHROMIUM_CHLIBS}/printing ${CHROMIUM_CHLIBS}/v8/tools/gyp ${CHROMIUM_CHLIBS}/v8/src/extensions/experimental ${CHROMIUM_CHLIBS}/gpu ${CHROMIUM_CHLIBS}/ui/gfx ${CHROMIUM_CHLIBS}/gfx ${CHROMIUM_CHLIBS}/sdch ${CHROMIUM_CHLIBS}/build/temp_gyp  ${CHROMIUM_CHLIBS}/seccompsandbox ${CHROMIUM_CHLIBS}/third_party/undoview ${CHROMIUM_CHLIBS}/ppapi ${CHROMIUM_CHLIBS}/third_party/ffmpeg ${CHROMIUM_CHLIBS}/third_party/openmax ${CHROMIUM_CHLIBS}/third_party/harfbuzz ${CHROMIUM_CHLIBS}/third_party/libjingle ${CHROMIUM_CHLIBS}/third_party/hunspell ${CHROMIUM_CHLIBS}/third_party/icu ${CHROMIUM_CHLIBS}/third_party/libevent ${CHROMIUM_CHLIBS}/third_party/libxml ${CHROMIUM_CHLIBS}/third_party/libxslt ${CHROMIUM_CHLIBS}/third_party/modp_b64 ${CHROMIUM_CHLIBS}/third_party/protobuf ${CHROMIUM_CHLIBS}/third_party/cld ${CHROMIUM_CHLIBS}/third_party/ots ${CHROMIUM_CHLIBS}/third_party/angle/src ${CHROMIUM_CHLIBS}/base/third_party/dynamic_annotations ${CHROMIUM_CHLIBS}/third_party/sqlite ${CHROMIUM_CHLIBS}/third_party/zlib ${CHROMIUM_CHLIBS}/third_party/flac ${CHROMIUM_CHLIBS}/third_party/libwebp ${CHROMIUM_CHLIBS}/third_party/cacheinvalidation ${CHROMIUM_CHLIBS}/webkit/support ${CHROMIUM_CHLIBS}/third_party/WebKit/Source/WebKit/chromium ${CHROMIUM_CHLIBS}/third_party/WebKit/Source/JavaScriptCore/JavaScriptCore.gyp ${CHROMIUM_CHLIBS}/third_party/WebKit/Source/WebCore/WebCore.gyp ${CHROMIUM_CHLIBS}/content ${CHROMIUM_CHLIBS}/chrome/app/policy ${CHROMIUM_CHLIBS}/webkit/support)

    SET(CHROMIUM_TPLIBS ${CHROMIUM_CHLIBS}/third_party/openmax/libil.a event zlib png jpeg ${CHROMIUM_CHLIBS}/third_party/libjpeg_turbo/libjpeg_turbo.a xslt bz2 Xss ${CHROMIUM_CHLIBS}/third_party/sqlite/libsqlite3.a ${CHROMIUM_CHLIBS}/net/third_party/nss/libssl.a undoview allocator base_i18n xdg_mime seccomp_sandbox symbolize gcrypt)

    SET(CHROMIUM_GENINCLUDES ${CHROMIUM_CHLIBS}/gen/chrome)
  ELSE(NOT CHROMIUM_ISXCODE)
    ### XCode Build System (Mac OS X) ###
    SET(CHROMIUM_CHLIBS ${CHROMIUMLIBPATH})
    IF (NOT CHROMIUN_CHLIBS)
       SET(CHROMIUM_CHLIBS ${CHROMIUMDIR}/src/xcodebuild/${CHROMIUMMODE})
    ENDIF()
    SET(CHROMIUM_DATADIR ${CHROMIUM_CHLIBS})

    SET(CHROME_LIBRARY_DIRS ${CHROMIUM_CHLIBS})
    SET(CHROMIUM_TPLIBS resolv ${CHROMIUM_CHLIBS}/../chrome.build/Release/chrome_dll.build/Objects-normal/i386/breakpad_mac_stubs.o chrome_gpu chrome_zlib service_runtime_x86_32 webkit_system_interface iccjpeg gdb_rsp env_cleanser WebKitSystemInterface${SNOW}LeopardPrivateExtern ${CHROMIUM_CHLIBS}/libexpat.a ncvalidate ncopcode_utils service_runtime_x86_common ${CHROMIUM_CHLIBS}/libevent.a ${CHROMIUM_CHLIBS}/libxslt.a ${CHROMIUM_CHLIBS}/libjpeg_turbo.a ${CHROMIUM_CHLIBS}/libpng.a ${CHROMIUM_CHLIBS}/libssl.a ${CHROMIUM_CHLIBS}/libxml2.a ${CHROMIUM_CHLIBS}/libsqlite3.a ${CHROMIUM_CHLIBS}/libnss.a ${CHROMIUM_CHLIBS}/libnspr.a chrome_bz2 imc)
    SET(GENINCLUDES ${CHROMIUMDIR}/src/xcodebuild/DerivedSources/${CHROMIUMMODE}/chrome)

  ENDIF(NOT CHROMIUM_ISXCODE)
ELSE(NOT CHROMIUM_ISSCONS)
  ### SCONS Build System (deprecated) ###
  SET(CHROMIUM_DATADIR ${CHROMIUMDIR}/src/out/${CHROMIUMMODE})
  IF(NOT CHROMIUMLIBPATH)
    SET(CHROMIUMLIBPATH ${CHROMIUM_DATADIR}/lib)
  ENDIF()
  SET(GENINCLUDES ${CHROMIUM_DATADIR}) #not sure

  SET(CHROME_LIBRARY_DIRS ${CHROMIUMLIBPATH})
  SET(CHROMIUM_TPLIBS event xslt jpeg png z bz2 ${CHROMIUMLIBPATH}/libsqlite.a google_nacl_imc_c  base_i18n ${CHROMIUM_CHLIBS}/third_party/libjpeg_turbo/libjpeg_turbo.a  gcrypt)
ENDIF(NOT CHROMIUM_ISSCONS)

SET(CHROMIUMLIBS ${CHROMIUMLDFLAGS} webcore_svg webcore_bindings webcore_remaining webcore_platform webcore_html ${CHROMIUM_TPLIBS} gfx dl m common common_net dynamic_annotations browser debugger renderer utility printing app_base appcache base base_i18n protobuf_lite glue icui18n installer_util database syncapi sync icuuc icudata skia skia_opts xml2 net net_base cld ots googleurl sdch modp_b64 v8_snapshot v8_base wtf webkit fileapi webkit_user_agent blob speex media ffmpeg http_server cacheinvalidation chromoting_base chromoting_client chromoting_host chromoting_protocol chromoting_jingle_glue chromotocol_proto_lib trace_proto_lib jingle_p2p chromoting_plugin notifier ppapi_cpp_objects ppapi_proxy ppapi_shared_impl sync_notifier gles2_implementation gles2_c_lib command_buffer_client command_buffer_service command_buffer_common gles2_cmd_helper chrome_gpu hunspell plugin ipc worker common_constants default_plugin jingle translator_common translator_glsl profile_import service policy webp flac gpu_ipc yuv_convert_sse2 yuv_convert yarr i18n_api cpu_features webkit_gpu cups pam quota content_browser content_common webcore_rendering)
IF (APPLE OR LINUX_ENABLE_NACL)
  SET(CHROMIUMLIBS ${CHROMIUMLIBS} nacl nacl_base nacl_ppapi_browser nonnacl_srpc nonnacl_util sel sel_ldr_launcher gio gio_wrapped_desc platform nrd_xfer debug_stub platform_qual_lib)
ENDIF()
SET(CHROMIUM_ARCHFLAGS)
# Flags that affect both compiling and linking
SET(CHROMIUM_CLIBFLAGS ${CHROMIUM_ARCHFLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -fPIC -pthread -fno-rtti)
SET(CHROME_INCLUDE_DIRS ${GENINCLUDES} ${CHROMIUMDIR}/src/ ${CHROMIUMDIR}/src/third_party/npapi ${CHROMIUMDIR}/src/third_party/WebKit/Source/JavaScriptCore ${CHROMIUMDIR}/src/third_party/icu/public/common ${CHROMIUMDIR}/src/skia/config ${CHROMIUMDIR}/src/third_party/skia/include/core ${CHROMIUMDIR}/src/third_party/WebKit/Source/WebKit/chromium/public ${CHROMIUMDIR}/src/third_party/WebKit/Source/WebCore/platform/text)
SET(CHROME_CFLAGS ${CHROMIUM_DEBUGFLAGS} ${CHROMIUM_CLIBFLAGS} ${CHROMIUM_PLAT_CFLAGS} -Wall -Wno-non-virtual-dtor -DNVALGRIND -D_REENTRANT -D__STDC_FORMAT_MACROS -DCHROMIUM_BUILD -DU_STATIC_IMPLEMENTATION -g )

SET(CHROME_LIBRARIES ${CHROMIUM_START_GROUP} ${CHROMIUM_PLAT_LDFLAGS} ${CHROMIUMLIBS} ${CHROMIUM_END_GROUP})
SET(CHROME_LDFLAGS ${AS_NEEDED_FLAG} -g -shared ${CHROMIUM_CLIBFLAGS})
#OBJDIR=$(CHROMIUMMODE)
#EXEDIR=$(CHROMIUMMODE)

FOREACH(CHROMIUMLIB ${CHROME_LIBRARY_DIRS})
  IF(NOT EXISTS ${CHROMIUMLIB})
    IF(CHROME_FOUND)
      MESSAGE(STATUS "Failed to find Chrome library directory at ${CHROMIUMLIB}")
    ENDIF()
    SET(CHROME_FOUND FALSE)
  ENDIF()
ENDFOREACH()
FOREACH(CHROMIUMINC ${CHROME_INCLUDE_DIRS})
  IF(NOT EXISTS ${CHROMIUMINC})
    IF(CHROME_FOUND)
      MESSAGE(STATUS "Failed to find Chrome include directory at ${CHROMIUMINC}")
    ENDIF()
    SET(CHROME_FOUND FALSE)
  ENDIF()
ENDFOREACH()

IF(CHROME_FOUND)
  IF(APPLE)
    MACRO(ADD_CHROME_APP)
      PARSE_ARGUMENTS(CHROME_APP "APP;DEPENDS;LINKS;COPY" "" ${ARGN})

      IF(NOT CHROME_APP_APP)
        MESSAGE(ERROR "No application name specified as parameter to ADD_CHROME_APP")
      ELSE()
        SET(CHROME_APP_NAME ${CHROME_APP_APP}.app)
      ENDIF()

      SET(CHROMIUM_FRAMEWORK Chromium\ Framework.framework)
      SET(CHROME_LANG en)
      SET(CHROMIUM_BIN_BUILD_PATH ${CHROME_ROOT}/src/xcodebuild/${CHROMIUMMODE})
      SET(CHROMIUM_FRAMEWORK_PATH ${CHROMIUM_BIN_BUILD_PATH}/${CHROMIUM_FRAMEWORK})
      SET(LOCAL_CHROMIUM_FRAMEWORK ${CHROME_APP_NAME}/Contents/lib/${CHROMIUM_FRAMEWORK})
      SET(CHROME_SYMLINKS_COMMAND
        mkdir -p ${CHROME_APP_NAME} &&
        mkdir -p ${CHROME_APP_NAME}/Contents &&
        mkdir -p ${CHROME_APP_NAME}/Contents/lib &&
        mkdir -p ${CHROME_APP_NAME}/Contents/MacOS &&
        mkdir -p ${LOCAL_CHROMIUM_FRAMEWORK} &&
        mkdir -p ${LOCAL_CHROMIUM_FRAMEWORK}/Libraries &&
        mkdir -p ${LOCAL_CHROMIUM_FRAMEWORK}/Resources &&
        sh ${BERKELIUM_TOP_LEVEL}/util/make-info-plist.sh ${CHROME_ROOT} ${CHROME_APP_NAME} ${CHROME_APP_APP} org.berkelium.${CHROME_APP_APP} &&
        ln -sf ${CHROMIUM_BIN_BUILD_PATH}/libplugin_carbon_interpose.dylib ${CHROME_APP_NAME}/Contents/MacOS/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Libraries/libffmpegsumo.dylib ${LOCAL_CHROMIUM_FRAMEWORK}/Libraries/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Resources/chrome.pak ${LOCAL_CHROMIUM_FRAMEWORK}/Resources/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Resources/${CHROME_LANG}.lproj ${LOCAL_CHROMIUM_FRAMEWORK}/Resources/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Resources/utility.sb ${LOCAL_CHROMIUM_FRAMEWORK}/Resources/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Resources/nacl_loader.sb ${LOCAL_CHROMIUM_FRAMEWORK}/Resources/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Resources/worker.sb ${LOCAL_CHROMIUM_FRAMEWORK}/Resources/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Resources/common.sb ${LOCAL_CHROMIUM_FRAMEWORK}/Resources/ &&
        ln -sf ${CHROMIUM_FRAMEWORK_PATH}/Resources/renderer.sb ${LOCAL_CHROMIUM_FRAMEWORK}/Resources/
        )

      SET(CHROME_SYMLINKS_COMMAND
        ${CHROME_SYMLINKS_COMMAND} &&
        ln -sf ${CMAKE_CURRENT_BINARY_DIR}/${CHROME_APP_APP} ${CHROME_APP_NAME}/Contents/MacOS/
        )
      FOREACH(CHROME_SYMLINK_BINARY ${CHROME_APP_LINKS})
        SET(CHROME_SYMLINKS_COMMAND
          ${CHROME_SYMLINKS_COMMAND} &&
          ln -sf ${CHROME_SYMLINK_BINARY} ${CHROME_APP_NAME}/Contents/MacOS/
          )
      ENDFOREACH()
      FOREACH(CHROME_COPY_BINARY ${CHROME_APP_COPY})
        SET(CHROME_SYMLINKS_COMMAND
          ${CHROME_SYMLINKS_COMMAND} &&
          (cp -f ${CHROME_COPY_BINARY} ${CHROME_APP_NAME}/Contents/MacOS/ || true)
          )
      ENDFOREACH()


      SET(DEPENDS_ARGS)
      IF(CHROME_APP_DEPENDS)
        SET(DEPENDS_ARGS DEPENDS ${CHROME_APP_DEPENDS})
      ENDIF()
      ADD_CUSTOM_TARGET(${CHROME_APP_NAME} ALL
        COMMAND ${CHROME_SYMLINKS_COMMAND}
        ${DEPENDS_ARGS}
        )
    ENDMACRO(ADD_CHROME_APP)
  ELSE()
    MACRO(ADD_CHROME_SYMLINK_TARGET)
      PARSE_ARGUMENTS(CHROME_SYMLINKS_ARG "TARGETNAME;DEPENDS" "" ${ARGN})

      SET(CHROME_SYMLINKS_COMMAND_TARGET chrome_symlinks)
      IF(CHROME_SYMLINKS_ARG_TARGETNAME)
        SET(CHROME_SYMLINKS_COMMAND_TARGET ${CHROME_SYMLINKS_ARG_TARGETNAME})
      ENDIF()

      SET(CHROME_SYMLINKS_COMMAND
        ln -sf ${CHROMIUM_DATADIR}/locales &&
        ln -sf ${CHROMIUM_DATADIR}/resources &&
        ln -sf ${CHROMIUM_DATADIR}/resources.pak &&
        ln -sf ${CHROMIUM_DATADIR}/chrome.pak
        )

      SET(DEPENDS_ARGS)
      IF(CHROME_SYMLINKS_ARG_DEPENDS)
        SET(DEPENDS_ARGS DEPENDS ${CHROME_SYMLINKS_ARG_DEPENDS})
      ENDIF()

      ADD_CUSTOM_TARGET(${CHROME_SYMLINKS_COMMAND_TARGET} ALL
        COMMAND ${CHROME_SYMLINKS_COMMAND}
        ${DEPENDS_ARGS}
        )
    ENDMACRO()
  ENDIF()


  IF(NOT CHROME_FOUND_QUIETLY)
    MESSAGE(STATUS "Found Chrome: headers at ${CHROME_INCLUDE_DIRS}, libraries at ${CHROME_LIBRARY_DIRS}")
    MESSAGE(STATUS "Found Chrome: LDFLAGS at ${CHROME_LDFLAGS}")
    MESSAGE(STATUS "Found Chrome: CFLAGS at ${CHROME_CFLAGS}")
  ELSE()
    MESSAGE(STATUS "Chrome Found ")
  ENDIF()
ENDIF(CHROME_FOUND)
