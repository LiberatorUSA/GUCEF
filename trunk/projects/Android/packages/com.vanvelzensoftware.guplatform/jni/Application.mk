# Developing native apps radically changed in NDK r5 with the native activity thus we
# only support Gingerbread and above in this code.

APP_PLATFORM := android-9

# The following enables exception support for all modules in this application.
# Note that it is said that exception support does require exceptions to inherit from std::exception
# which means you also need NDK r5's STLport

APP_CPPFLAGS += -fexceptions

# the NDK toolchain supports C++ RTTI (RunTime Type Information)
# since NDK r5, but all C++ sources are built with -fno-rtti by default for
# compatibility reasons. To enable it, add the following to your module
# declarations:

#APP_CPPFLAGS += -frtti

# STL
#
# By default, the headers and libraries for the minimal C++ runtime system
# library (/system/lib/libstdc++.so) are used when building C++ sources.
#
# You can however select a different implementation by setting the variable
# APP_STL to something else in your Application.mk, for example:
# 
#   APP_STL := stlport_static
# 
# To select the static STLport implementation provided with this NDK.
# Value APP_STL values are the following:
# 
#    system              -> Use the default minimal C++ runtime library.
#    stlport_static      -> Use STLport built as a static library.
#    stlport_shared      -> Use STLport built as a shared library.
#    gnustl_static       -> Use GNU libstdc++ as a static library.
# 
# WARNING: IMPORTANT CAVEAT
# 
#     AT THE MOMENT, OUR STLPORT IMPLEMENTATION DOES NOT SUPPORT EXCEPTIONS
#     AND RTTI. PLEASE BE SURE TO NOT USE -fexceptions OR -frtti IN ALL
#     MODULES THAT USE IT.
#
#     IF YOU NEED THESE, PLEASE USE "gnustl_static".
#
# WARNING: END OF IMPORTANT CAVEAT
#
#  "stlport_shared" is preferred if you have several shared libraries in your
#  project that use the C++ STL, because it avoids duplication of functions
#  and more importantly of global variables (e.g. std::cout) in each one of
#  them, which can have surprising results.
#
#  On the other hand, you will have to load it explicitely when starting your
#  application, as in the following example:
#
#     static {
#         System.loadLibrary("stlport_shared");
#         System.loadLibrary("foo");
#         System.loadLibrary("bar");
#     }
#
#  Where both "libfoo.so" and "libbar.so" depend on "libstlport_shared.so".
#
#  Note that the shared library's name if "libstlport_shared.so" to avoid
#  naming conflicts with certain Android system images which include a
#  system-level libstlport.so (which happens to not be ABI-stable and
#  cannot be used from NDK-generated machine code).
#
#  "stlport_static" is preferred if you have only one shared library in your
#  project: only the STL functions and variables you actually need will be
#  linked to your machine code, reducing its code size, and you won't need
#  to load the dynamic stlport_shared at startup.
#
#
#  "gnustl_static" is preferred (and required) if you need C++ Exceptions
#  and RTTI support at the moment.
#
#
#  IV. STLport-specific issues:
#  ----------------------------
#
# This NDK provides prebuilt static and shared libraries for STLport,
# but you can force it to be rebuilt from sources by defining the following
# in your environment or your Application.mk before building:
#
#    STLPORT_FORCE_REBUILD := true
#
# STLport is licensed under a BSD-style open-source license. See
# sources/cxx-stl/stlport/README for more details about the library.
#

APP_STL := gnustl_static
