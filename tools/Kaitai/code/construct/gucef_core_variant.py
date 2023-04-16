from construct import *
from construct.lib import *
import enum

class gucef_core_variant__gucef_datatypes(enum.IntEnum):
	uint8 = 1
	int8 = 2
	uint16 = 3
	int16 = 5
	uint32 = 7
	int32 = 9
	uint64 = 11
	int64 = 13
	float32 = 15
	float64 = 17
	ascii_string = 21
	utf8_string = 22
	utf16_le_string = 23
	utf16_be_string = 24
	utf32_string = 25
	boolean_int32 = 30
	boolean_ascii_string = 31
	boolean_utf8_string = 32
	binary_blob = 40
	binary_bsob = 41
	datetime_iso8601_ascii_string = 50
	datetime_iso8601_utf8_string = 51
	le_timestamp_in_secs_since_unix_epoch = 55
	be_timestamp_in_secs_since_unix_epoch = 56
	le_timestamp_in_ms_since_unix_epoch = 57
	be_timestamp_in_ms_since_unix_epoch = 58

gucef_core_variant__gucef_int64 = Struct(
	'data' / Int64sl,
)

gucef_core_variant__gucef_bsob = Struct(
	'data' / Int8ub,
)

gucef_core_variant__gucef_be_timestamp_in_ms_since_unix_epoch = Struct(
	'data' / Int64ul,
)

gucef_core_variant__gucef_le_timestamp_in_secs_since_unix_epoch = Struct(
	'data' / Int64ul,
)

gucef_core_variant__gucef_uint16 = Struct(
	'data' / Int16ul,
)

gucef_core_variant__gucef_uint64 = Struct(
	'data' / Int64ul,
)

gucef_core_variant__gucef_uint8 = Struct(
	'data' / Int8ub,
)

gucef_core_variant__gucef_int8 = Struct(
	'data' / Int8sb,
)

gucef_core_variant__gucef_int16 = Struct(
	'data' / Int16sl,
)

gucef_core_variant__gucef_float64 = Struct(
	'data' / Float64l,
)

gucef_core_variant__gucef_le_timestamp_in_ms_since_unix_epoch = Struct(
	'data' / Int64ul,
)

gucef_core_variant__gucef_int32 = Struct(
	'data' / Int32sl,
)

gucef_core_variant__gucef_be_timestamp_in_secs_since_unix_epoch = Struct(
	'data' / Int64ul,
)

gucef_core_variant__gucef_float32 = Struct(
	'data' / Float32l,
)

gucef_core_variant__gucef_uint32 = Struct(
	'data' / Int32ul,
)

gucef_core_variant__gucef_boolean_int32 = Struct(
	'data' / Int32sl,
)

gucef_core_variant = Struct(
	'type_id' / Enum(Int8ub, gucef_core_variant__gucef_datatypes),
	'payload' / Switch(this.type_id, {'datetime_iso8601_ascii_string': LazyBound(lambda: gucef_core_dynamic), 'utf16_le_string': LazyBound(lambda: gucef_core_dynamic), 'int16': LazyBound(lambda: gucef_core_variant__gucef_int16), 'float32': LazyBound(lambda: gucef_core_variant__gucef_float32), 'ascii_string': LazyBound(lambda: gucef_core_dynamic), 'uint16': LazyBound(lambda: gucef_core_variant__gucef_uint16), 'uint64': LazyBound(lambda: gucef_core_variant__gucef_uint64), 'int32': LazyBound(lambda: gucef_core_variant__gucef_int32), 'uint8': LazyBound(lambda: gucef_core_variant__gucef_uint8), 'uint32': LazyBound(lambda: gucef_core_variant__gucef_uint32), 'float64': LazyBound(lambda: gucef_core_variant__gucef_float64), 'be_timestamp_in_secs_since_unix_epoch': LazyBound(lambda: gucef_core_variant__gucef_be_timestamp_in_secs_since_unix_epoch), 'be_timestamp_in_ms_since_unix_epoch': LazyBound(lambda: gucef_core_variant__gucef_be_timestamp_in_ms_since_unix_epoch), 'utf32_string': LazyBound(lambda: gucef_core_dynamic), 'boolean_utf8_string': LazyBound(lambda: gucef_core_dynamic), 'binary_blob': LazyBound(lambda: gucef_core_dynamic), 'le_timestamp_in_ms_since_unix_epoch': LazyBound(lambda: gucef_core_variant__gucef_le_timestamp_in_ms_since_unix_epoch), 'binary_bsob': LazyBound(lambda: gucef_core_variant__gucef_bsob), 'boolean_ascii_string': LazyBound(lambda: gucef_core_dynamic), 'utf8_string': LazyBound(lambda: gucef_core_dynamic), 'int8': LazyBound(lambda: gucef_core_variant__gucef_int8), 'boolean_int32': LazyBound(lambda: gucef_core_variant__gucef_boolean_int32), 'le_timestamp_in_secs_since_unix_epoch': LazyBound(lambda: gucef_core_variant__gucef_le_timestamp_in_secs_since_unix_epoch), 'datetime_iso8601_utf8_string': LazyBound(lambda: gucef_core_dynamic), 'int64': LazyBound(lambda: gucef_core_variant__gucef_int64), 'utf16_be_string': LazyBound(lambda: gucef_core_dynamic), }),
)

_schema = gucef_core_variant
