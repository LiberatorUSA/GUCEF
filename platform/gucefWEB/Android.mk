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
LOCAL_MODULE := gucefWEB
LOCAL_MODULE_FILENAME := libgucefWEB
@echo Module name: $(LOCAL_MODULE)

LOCAL_SRC_FILES := \
  src/gucefWEB.cpp \
  src/gucefWEB_CAsyncHttpServerRequestHandler.cpp \
  src/gucefWEB_CAsyncHttpServerResponseHandler.cpp \
  src/gucefWEB_CCodecBasedHTTPServerResource.cpp \
  src/gucefWEB_CConfigurableHttpServerResource.cpp \
  src/gucefWEB_CDefaultHTTPServerResource.cpp \
  src/gucefWEB_CDefaultHTTPServerRouter.cpp \
  src/gucefWEB_CDefaultHTTPServerRouterController.cpp \
  src/gucefWEB_CDefaultHttpServerRequestHandler.cpp \
  src/gucefWEB_CDummyHTTPServerResource.cpp \
  src/gucefWEB_CGlobalHttpCodecLinks.cpp \
  src/gucefWEB_CHTTPClient.cpp \
  src/gucefWEB_CHTTPServer.cpp \
  src/gucefWEB_CHTTPURLHandler.cpp \
  src/gucefWEB_CHttpCodecLinks.cpp \
  src/gucefWEB_CHttpMimeTypes.cpp \
  src/gucefWEB_CHttpRequestData.cpp \
  src/gucefWEB_CHttpResponseData.cpp \
  src/gucefWEB_CIHTTPServerResource.cpp \
  src/gucefWEB_CIHTTPServerRouter.cpp \
  src/gucefWEB_CIHTTPServerRouterController.cpp \
  src/gucefWEB_CIHttpServerRequestHandler.cpp \
  src/gucefWEB_CModule.cpp \
  src/gucefWEB_CWebGlobal.cpp

LOCAL_C_INCLUDES := \
  $(MY_MODULE_PATH)/include \
  $(MY_MODULE_PATH)/../../common/include \
  $(MY_MODULE_PATH)/../gucefCOM/include \
  $(MY_MODULE_PATH)/../gucefCOMCORE/include \
  $(MY_MODULE_PATH)/../gucefCORE/include \
  $(MY_MODULE_PATH)/../gucefCORE/include/android \
  $(MY_MODULE_PATH)/../gucefMT/include \
  $(MY_MODULE_PATH)/../gucefVFS/include

LOCAL_CFLAGS := -DGUCEF_WEB_BUILD_MODULE


LOCAL_SHARED_LIBRARIES := \
  gucefCOM \
  gucefCOMCORE \
  gucefCORE \
  gucefMT \
  gucefVFS

include $(BUILD_SHARED_LIBRARY)
