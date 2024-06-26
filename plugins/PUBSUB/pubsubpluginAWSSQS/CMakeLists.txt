#-------------------------------------------------------------------
# This file was automatically generated by ProjectGenerator
# which is tooling part the build system designed for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------
#

# Configuration for module: pubsubpluginAWSSQS

set( HEADER_FILES 
  include/pubsubpluginAWSSQS.h
  include/pubsubpluginAWSSQS_CAwsSqsPubSubClient.h
  include/pubsubpluginAWSSQS_CAwsSqsPubSubClientTopic.h
  include/pubsubpluginAWSSQS_CAwsSqsPubSubClientTopicConfig.h
  include/pubsubpluginAWSSQS_config.h
  include/pubsubpluginAWSSQS_macros.h
)

set( SOURCE_FILES 
  src/pubsubpluginAWSSQS.cpp
  src/pubsubpluginAWSSQS_CAwsSqsPubSubClient.cpp
  src/pubsubpluginAWSSQS_CAwsSqsPubSubClientTopic.cpp
  src/pubsubpluginAWSSQS_CAwsSqsPubSubClientTopicConfig.cpp
)

# Make sure the PLATFORM_HEADER_FILES variable is always defined
 set( PLATFORM_HEADER_FILES "" )

# Make sure the PLATFORM_SOURCE_FILES variable is always defined
 set( PLATFORM_SOURCE_FILES "" )

# For ease of use make a variable that has all files for this module
set( ALL_FILES ${HEADER_FILES} ${SOURCE_FILES} ${PLATFORM_HEADER_FILES} ${PLATFORM_SOURCE_FILES} )

set( MODULE_NAME "pubsubpluginAWSSQS" )
add_library( ${MODULE_NAME} ${ALL_FILES} )
add_dependencies( ${MODULE_NAME} aws-cpp-sdk-sqs gucefCOMCORE gucefCORE gucefMT gucefPUBSUB pluginglueAWSSDK )
target_link_libraries( ${MODULE_NAME} aws-cpp-sdk-sqs gucefCOMCORE gucefCORE gucefMT gucefPUBSUB pluginglueAWSSDK )
set_property( TARGET ${MODULE_NAME} APPEND_STRING PROPERTY COMPILE_DEFINITIONS "PUBSUBPLUGIN_AWSSQS_BUILD_PLUGIN_DLL;USE_IMPORT_EXPORT" )

include_directories( ../../../common/include ../../../dependencies/aws-cpp-sdk-core/include ../../../dependencies/aws-cpp-sdk-core/include/aws ../../../dependencies/aws-cpp-sdk-core/include/aws/core ../../../dependencies/aws-cpp-sdk-core/include/aws/core/auth ../../../dependencies/aws-cpp-sdk-core/include/aws/core/auth/bearer-token-provider ../../../dependencies/aws-cpp-sdk-core/include/aws/core/auth/signer ../../../dependencies/aws-cpp-sdk-core/include/aws/core/auth/signer-provider ../../../dependencies/aws-cpp-sdk-core/include/aws/core/client ../../../dependencies/aws-cpp-sdk-core/include/aws/core/config ../../../dependencies/aws-cpp-sdk-core/include/aws/core/config/defaults ../../../dependencies/aws-cpp-sdk-core/include/aws/core/endpoint ../../../dependencies/aws-cpp-sdk-core/include/aws/core/endpoint/internal ../../../dependencies/aws-cpp-sdk-core/include/aws/core/external ../../../dependencies/aws-cpp-sdk-core/include/aws/core/external/cjson ../../../dependencies/aws-cpp-sdk-core/include/aws/core/external/tinyxml2 ../../../dependencies/aws-cpp-sdk-core/include/aws/core/http ../../../dependencies/aws-cpp-sdk-core/include/aws/core/http/curl ../../../dependencies/aws-cpp-sdk-core/include/aws/core/http/standard ../../../dependencies/aws-cpp-sdk-core/include/aws/core/internal ../../../dependencies/aws-cpp-sdk-core/include/aws/core/monitoring ../../../dependencies/aws-cpp-sdk-core/include/aws/core/net ../../../dependencies/aws-cpp-sdk-core/include/aws/core/platform ../../../dependencies/aws-cpp-sdk-core/include/aws/core/platform/refs ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/base64 ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/crypto ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/crypto/bcrypt ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/crypto/commoncrypto ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/crypto/openssl ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/event ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/json ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/logging ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/memory ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/memory/stl ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/ratelimiter ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/stream ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/threading ../../../dependencies/aws-cpp-sdk-core/include/aws/core/utils/xml ../../../dependencies/aws-cpp-sdk-sqs/include ../../../dependencies/aws-cpp-sdk-sqs/include/aws ../../../dependencies/aws-cpp-sdk-sqs/include/aws/sqs ../../../dependencies/aws-cpp-sdk-sqs/include/aws/sqs/model ../../../dependencies/aws-crt-cpp/crt/aws-c-auth/include ../../../dependencies/aws-crt-cpp/crt/aws-c-cal/include ../../../dependencies/aws-crt-cpp/crt/aws-c-common/include ../../../dependencies/aws-crt-cpp/crt/aws-c-compression/include ../../../dependencies/aws-crt-cpp/crt/aws-c-event-stream/include ../../../dependencies/aws-crt-cpp/crt/aws-c-http/include ../../../dependencies/aws-crt-cpp/crt/aws-c-io/include ../../../dependencies/aws-crt-cpp/crt/aws-c-io/source ../../../dependencies/aws-crt-cpp/crt/aws-c-mqtt/include ../../../dependencies/aws-crt-cpp/crt/aws-c-s3/include ../../../dependencies/aws-crt-cpp/crt/aws-c-sdkutils/include ../../../dependencies/aws-crt-cpp/crt/aws-checksums/include ../../../dependencies/aws-crt-cpp/include ../../../dependencies/curl/include ../../../dependencies/curl/include/curl ../../../dependencies/curl/lib ../../../dependencies/curl/lib/vauth ../../../dependencies/curl/lib/vquic ../../../dependencies/curl/lib/vssh ../../../dependencies/curl/lib/vtls ../../../platform/gucefCOM/include ../../../platform/gucefCOMCORE/include ../../../platform/gucefCORE/include ../../../platform/gucefMT/include ../../../platform/gucefPUBSUB/include ../../../platform/gucefVFS/include ../../../platform/gucefWEB/include ../../SHARED/pluginglueAWSSDK/include include )

if ( ANDROID )
  include_directories( ../../../platform/gucefCORE/include/android )
endif( ANDROID )

if ( LINUX32 )
  include_directories( ../../../platform/gucefCORE/include/linux )
endif( LINUX32 )

if ( LINUX64 )
  include_directories( ../../../platform/gucefCORE/include/linux )
endif( LINUX64 )

if ( WIN32 )
  include_directories( ../../../platform/gucefCOMCORE/include/mswin ../../../platform/gucefCORE/include/mswin ../../../platform/gucefMT/include/mswin )
endif( WIN32 )

if ( WIN64 )
  include_directories( ../../../platform/gucefCOMCORE/include/mswin ../../../platform/gucefCORE/include/mswin ../../../platform/gucefMT/include/mswin )
endif( WIN64 )



#******** START OF AUTOGENERATED TEMPLATED ADDITION CONTENT ********




if (ANDROID)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (LINUX32)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (LINUX64)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (OSX)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (WIN32)
GUCEF_config_lib( ${MODULE_NAME} )
elseif (WIN64)
GUCEF_config_lib( ${MODULE_NAME} )
else()
GUCEF_config_lib( ${MODULE_NAME} )
endif()



#******** END OF AUTOGENERATED TEMPLATED ADDITION CONTENT ********


