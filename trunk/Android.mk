#-------------------------------------------------------------------
# This file has been automatically generated by ProjectGenerator    
# which is part of a build system designed for GUCEF                
#     (Galaxy Unlimited Framework)                                  
# For the latest info, see http://www.VanvelzenSoftware.com/        
#                                                                   
# The contents of this file are placed in the public domain. Feel   
# free to make use of it in any way you like.                       
#-------------------------------------------------------------------


#
# This is the project makefile which includes all modules which are part of this project
#
# PROJECT: "GUCEF"
#

ifndef $(PROJECT_ROOT_PATH)
  PROJECT_ROOT_PATH := $(call my-dir)
endif

include $(CLEAR_VARS)

MY_MODULE_PATH := $(PROJECT_ROOT_PATH)/gucefMT
include $(MY_MODULE_PATH)/Android.mk

MY_MODULE_PATH := $(PROJECT_ROOT_PATH)/gucefCORE
include $(MY_MODULE_PATH)/Android.mk

MY_MODULE_PATH := $(PROJECT_ROOT_PATH)/gucefIMAGE
include $(MY_MODULE_PATH)/Android.mk

MY_MODULE_PATH := $(PROJECT_ROOT_PATH)/gucefVFS
include $(MY_MODULE_PATH)/Android.mk

MY_MODULE_PATH := $(PROJECT_ROOT_PATH)/gucefGUI
include $(MY_MODULE_PATH)/Android.mk

MY_MODULE_PATH := $(PROJECT_ROOT_PATH)/gucefINPUT
include $(MY_MODULE_PATH)/Android.mk