from construct import *
from construct.lib import *

gucef_pubsub_msgcontainer_footer = Struct(
	'msg_offset_idx' / Array(this.mois, Int32ul),
	'msg_offset_idx_size' / Int32ul,
	'magic_text' / FixedSized(10, GreedyBytes),
	'mois' / Pointer((stream_size(_io) - 14), Int32ul),
)

_schema = gucef_pubsub_msgcontainer_footer
