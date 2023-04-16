from construct import *
from construct.lib import *

gucef_core_dynamic = Struct(
	'len_data' / Int32ul,
	'data' / FixedSized(this.len_data, GreedyBytes),
)

_schema = gucef_core_dynamic
