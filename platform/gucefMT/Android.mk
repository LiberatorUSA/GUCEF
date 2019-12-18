#-------------------------------------------------------------------
# This file has been automatically generated by ProjectGenerator    
# which is part of a build system designed for GUCEF                
#     (Galaxy Unlimited Framework)                                  
# For the latest info, see http://www.VanvelzenSoftware.com/        
#                                                                   
# The contents of this file are placed in the public domain. Feel   
# free to make use of it in any way you like.                       
#-------------------------------------------------------------------


ifndef MY_MODULE_PATH
  MY_MODULE_PATH := $(call my-dir)
endif
LOCAL_PATH := $(MY_MODULE_PATH)

include $(CLEAR_VARS)

@echo Module path: $(MY_MODULE_PATH)
LOCAL_MODULE := gucefMT
LOCAL_MODULE_FILENAME := libgucefMT
@echo Module name: $(LOCAL_MODULE)

LOCAL_SRC_FILES := \
  src/gucefMT.cpp \
  src/gucefMT_CActiveObject.cpp \
  src/gucefMT_CCondition.cpp \
  src/gucefMT_CCriticalSection.cpp \
  src/gucefMT_CGUCEFMTModule.cpp \
  src/gucefMT_CICloneable.cpp \
  src/gucefMT_CMutex.cpp \
  src/gucefMT_CReadWriteLock.cpp \
  src/gucefMT_CScopeMutex.cpp \
  src/gucefMT_CSemaphore.cpp \
  src/gucefMT_CTMailBox.cpp \
  src/gucefMT_dvmtoswrap.c \
  src/gucefMT_DVRWLOCK.c \
  src/gucefMT_MUTEX.c

LOCAL_C_INCLUDES := \
  $(MY_MODULE_PATH)/include \
  $(MY_MODULE_PATH)/../common/include

LOCAL_CFLAGS := -DGUCEF_MT_BUILD_MODULE

include $(BUILD_SHARED_LIBRARY)
