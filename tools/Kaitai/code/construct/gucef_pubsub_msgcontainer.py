from construct import *
from construct.lib import *

gucef_pubsub_msgcontainer__gucef_pubsub_msg = Struct(
	'msg_date_time' / If((this._root.header.options & (1 << 1)) != 0, Switch((this._root.header.options & (1 << 2)), {0: Int32ul, }, default=Int64ul)),
	'msgid' / If((this._root.header.options & (1 << 3)) != 0, LazyBound(lambda: gucef_core_variant)),
	'msg_index' / If((this._root.header.options & (1 << 4)) != 0, LazyBound(lambda: gucef_core_variant)),
	'primary_payload_included' / If((this._root.header.options & (1 << 5)) != 0, LazyBound(lambda: gucef_core_variant)),
	'nr_of_kv_pairs' / If((this._root.header.options & (1 << 6)) != 0, Int32ul),
	'key_value_pairs' / If((this._root.header.options & (1 << 6)) != 0, Array(this.nr_of_kv_pairs, LazyBound(lambda: gucef_pubsub_msgcontainer__kv_pair))),
	'nr_of_metadata_kv_pairs' / If((this._root.header.options & (1 << 7)) != 0, Int32ul),
	'metadata_key_value_pairs' / If((this._root.header.options & (1 << 7)) != 0, Array(this.nr_of_metadata_kv_pairs, LazyBound(lambda: gucef_pubsub_msgcontainer__kv_pair))),
)

gucef_pubsub_msgcontainer__kv_pair = Struct(
	'key' / LazyBound(lambda: gucef_core_variant),
	'value' / LazyBound(lambda: gucef_core_variant),
)

gucef_pubsub_msgcontainer = Struct(
	'header' / LazyBound(lambda: gucef_pubsub_msgcontainer_header),
	'pubsubmsg' / RepeatUntil(lambda obj_, list_, this: stream_tell(_io) == (((stream_size(_io) - (this.msg_offset_idx_size * 4)) - 4) - 10), LazyBound(lambda: gucef_pubsub_msgcontainer__gucef_pubsub_msg)),
	'footer' / LazyBound(lambda: gucef_pubsub_msgcontainer_footer),
	'msg_offset_idx_size' / Pointer((stream_size(_io) - 14), Int32ul),
)

_schema = gucef_pubsub_msgcontainer
