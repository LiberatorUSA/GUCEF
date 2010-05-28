LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := gucefANDROIDGLUE
LOCAL_SRC_FILES := gucefANDROIDGLUE_GUCEFApplicationGlue.cpp

include $(BUILD_SHARED_LIBRARY)