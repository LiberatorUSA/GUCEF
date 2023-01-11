#
#   gucefPUBSUB: GUCEF module providing pub-sub communication facilities
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
  id: gucef_pubsub_msgcontainer
  endian: le
  imports:
  - gucef_core_variant    
  - gucef_pubsub_msgcontainer_header
  - gucef_pubsub_msgcontainer_footer
seq:
  - id: header
    type: gucef_pubsub_msgcontainer_header
  - id: pubsubmsg
    type: gucef_pubsub_msg
    repeat: until
    repeat-until: _io.pos == _io.size - (msg_offset_idx_size * 4) - 4 - 10
  - id: footer
    type: gucef_pubsub_msgcontainer_footer
instances:
  msg_offset_idx_size:
    pos: _io.size - 14
    type: u4
types:
  gucef_pubsub_msg:
    seq:
      - id: msg_date_time
        type:
          switch-on: (_root.header.options & (1 << 2))
          cases:
            0: u4
            _: u8
        if: (_root.header.options & (1 << 1)) != 0
      - id: msgid
        type: gucef_core_variant
        if: (_root.header.options & (1 << 3)) != 0
      - id: msg_index
        type: gucef_core_variant
        if: (_root.header.options & (1 << 4)) != 0
      - id: primary_payload_included
        type: gucef_core_variant
        if: (_root.header.options & (1 << 5)) != 0
      - id: nr_of_kv_pairs
        type: u4
        if: (_root.header.options & (1 << 6)) != 0
      - id: key_value_pairs
        type: kv_pair	
        repeat: expr
        repeat-expr: nr_of_kv_pairs
        if: (_root.header.options & (1 << 6)) != 0
      - id: nr_of_metadata_kv_pairs
        type: u4
        if: (_root.header.options & (1 << 7)) != 0
      - id: metadata_key_value_pairs
        type: kv_pair	
        repeat: expr
        repeat-expr: nr_of_metadata_kv_pairs
        if: (_root.header.options & (1 << 7)) != 0        
  kv_pair:
    seq:
      - id: key
        type: gucef_core_variant
      - id: value
        type: gucef_core_variant