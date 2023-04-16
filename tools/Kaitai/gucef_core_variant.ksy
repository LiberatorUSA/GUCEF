#
#   gucefCORE: GUCEF module providing O/S abstraction and generic solutions
#
#   Copyright (C) 1998 - 2022.  Dinand Vanvelzen
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
meta:
  id: gucef_core_variant
  endian: le
  imports:
  - gucef_core_dynamic
seq:
  - id: type_id
    type: u1
    enum: gucef_datatypes
  - id: payload    
    type:
      switch-on: type_id
      cases:
        'gucef_datatypes::uint8': gucef_uint8
        'gucef_datatypes::int8': gucef_int8
        'gucef_datatypes::uint16': gucef_uint16
        'gucef_datatypes::int16': gucef_int16
        'gucef_datatypes::uint32': gucef_uint32
        'gucef_datatypes::int32': gucef_int32
        'gucef_datatypes::uint64': gucef_uint64
        'gucef_datatypes::int64': gucef_int64
        'gucef_datatypes::float32': gucef_float32
        'gucef_datatypes::float64': gucef_float64
        'gucef_datatypes::boolean_int32': gucef_boolean_int32
        'gucef_datatypes::binary_bsob': gucef_bsob
        'gucef_datatypes::binary_blob': gucef_core_dynamic
        'gucef_datatypes::le_timestamp_in_secs_since_unix_epoch': gucef_le_timestamp_in_secs_since_unix_epoch
        'gucef_datatypes::be_timestamp_in_secs_since_unix_epoch': gucef_be_timestamp_in_secs_since_unix_epoch
        'gucef_datatypes::le_timestamp_in_ms_since_unix_epoch': gucef_le_timestamp_in_ms_since_unix_epoch
        'gucef_datatypes::be_timestamp_in_ms_since_unix_epoch': gucef_be_timestamp_in_ms_since_unix_epoch
        'gucef_datatypes::boolean_ascii_string' : gucef_core_dynamic
        'gucef_datatypes::boolean_utf8_string' : gucef_core_dynamic
        'gucef_datatypes::ascii_string' : gucef_core_dynamic
        'gucef_datatypes::utf8_string' : gucef_core_dynamic
        'gucef_datatypes::utf16_le_string' : gucef_core_dynamic
        'gucef_datatypes::utf16_be_string' : gucef_core_dynamic
        'gucef_datatypes::utf32_string' : gucef_core_dynamic
        'gucef_datatypes::datetime_iso8601_ascii_string' : gucef_core_dynamic
        'gucef_datatypes::datetime_iso8601_utf8_string' : gucef_core_dynamic
enums:
  gucef_datatypes:
    1: uint8
    2: int8
    3: uint16
    5: int16
    7: uint32
    9: int32
    11: uint64
    13: int64
    15: float32
    17: float64
    21: ascii_string
    22: utf8_string
    23: utf16_le_string
    24: utf16_be_string
    25: utf32_string
    30: boolean_int32
    31: boolean_ascii_string
    32: boolean_utf8_string    
    40: binary_blob
    41: binary_bsob
    50: datetime_iso8601_ascii_string
    51: datetime_iso8601_utf8_string
    55: le_timestamp_in_secs_since_unix_epoch
    56: be_timestamp_in_secs_since_unix_epoch
    57: le_timestamp_in_ms_since_unix_epoch 
    58: be_timestamp_in_ms_since_unix_epoch
types:
  gucef_uint8:
    seq:
        - id: data
          type: u1
  gucef_int8:
    seq:
        - id: data
          type: s1
  gucef_uint16:
    seq:
        - id: data
          type: u2
  gucef_int16:
    seq:
        - id: data
          type: s2
  gucef_uint32:
    seq:
        - id: data
          type: u4
  gucef_int32:
    seq:
        - id: data
          type: s4
  gucef_uint64:
    seq:
        - id: data
          type: u8
  gucef_int64:
    seq:
        - id: data
          type: s8
  gucef_float32:
    seq:
        - id: data
          type: f4
  gucef_float64:
    seq:
        - id: data
          type: f8
  gucef_boolean_int32:
    seq:
        - id: data
          type: s4
  gucef_bsob:
    seq:
        - id: data
          type: u1
  gucef_le_timestamp_in_secs_since_unix_epoch:
    seq:
        - id: data
          type: u8
  gucef_be_timestamp_in_secs_since_unix_epoch:
    seq:
        - id: data
          type: u8
  gucef_le_timestamp_in_ms_since_unix_epoch:
    seq:
        - id: data
          type: u8
  gucef_be_timestamp_in_ms_since_unix_epoch:
    seq:
        - id: data
          type: u8
