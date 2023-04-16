from construct import *
from construct.lib import *

gucef_pubsub_msgcontainer_header = Struct(
	'magic_text' / FixedSized(10, GreedyBytes),
	'version' / Int8ub,
	'options' / Int32ul,
)

_schema = gucef_pubsub_msgcontainer_header
