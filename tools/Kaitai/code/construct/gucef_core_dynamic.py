from construct import *
from construct.lib import *

gucef_core_dynamic = Struct(
	'payload_size' / Int32ul,
	'data' / FixedSized(this.payload_size, GreedyBytes),
)

_schema = gucef_core_dynamic
