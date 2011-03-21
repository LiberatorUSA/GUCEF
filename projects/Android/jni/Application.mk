# Developing native apps radically changed in NDK r5 with the native activity thus we
# only support Gingerbread and above in this code.
APP_PLATFORM := android-9

# The following enables exception support for all modules in this application.
# Note that it is said that exception support does require exceptions to inherit from std::exception
# which means you also need NDK r5's STLport
APP_CPPFLAGS += -fexceptions
