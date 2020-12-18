#
# This a redirect make file to the actual main make file
#

PROJECT_REDIRECT_ROOT_PATH := $(call my-dir)

ifndef MAINANDROIDMAKEDIR
  ifdef ANDROID_TARGET_PROJECT
    MAINANDROIDMAKEDIR := $(PROJECT_REDIRECT_ROOT_PATH)/../targets/$(ANDROID_TARGET_PROJECT)
  else
    MAINANDROIDMAKEDIR := $(PROJECT_REDIRECT_ROOT_PATH)/../../..
  endif
endif

include $(CLEAR_VARS)

PROJECT_ROOT_PATH := $(MAINANDROIDMAKEDIR)
include $(PROJECT_ROOT_PATH)/Android.mk
