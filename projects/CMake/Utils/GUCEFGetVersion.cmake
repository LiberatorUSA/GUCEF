#-------------------------------------------------------------------
# This file is part of the CMake build system for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

include(PreprocessorUtils)

macro(GUCEF_get_version HEADER)
  file(READ ${HEADER} TEMP_VAR_CONTENTS)
  get_preprocessor_entry(TEMP_VAR_CONTENTS GUCEF_VERSION_MAJOR GUCEF_VERSION_MAJOR)
  get_preprocessor_entry(TEMP_VAR_CONTENTS GUCEF_VERSION_MINOR GUCEF_VERSION_MINOR)
  get_preprocessor_entry(TEMP_VAR_CONTENTS GUCEF_VERSION_PATCH GUCEF_VERSION_PATCH)
  get_preprocessor_entry(TEMP_VAR_CONTENTS GUCEF_VERSION_NAME GUCEF_VERSION_NAME)
  get_preprocessor_entry(TEMP_VAR_CONTENTS GUCEF_VERSION_SUFFIX GUCEF_VERSION_SUFFIX)
  set(GUCEF_VERSION "${GUCEF_VERSION_MAJOR}.${GUCEF_VERSION_MINOR}.${GUCEF_VERSION_PATCH}")
endmacro()
