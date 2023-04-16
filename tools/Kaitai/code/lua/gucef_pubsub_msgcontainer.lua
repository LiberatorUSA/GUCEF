-- This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild
--
-- This file is compatible with Lua 5.3

local class = require("class")
require("kaitaistruct")

require("gucef_pubsub_msgcontainer_footer")
require("gucef_core_variant")
require("gucef_pubsub_msgcontainer_header")
GucefPubsubMsgcontainer = class.class(KaitaiStruct)

function GucefPubsubMsgcontainer:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefPubsubMsgcontainer:_read()
  self.header = GucefPubsubMsgcontainerHeader(self._io)
  self.pubsubmsg = {}
  local i = 0
  while true do
    local _ = GucefPubsubMsgcontainer.GucefPubsubMsg(self._io, self, self._root)
    self.pubsubmsg[i + 1] = _
    if self._io:pos() == (((self._io:size() - (self.msg_offset_idx_size * 4)) - 4) - 10) then
      break
    end
    i = i + 1
  end
  self.footer = GucefPubsubMsgcontainerFooter(self._io)
end

GucefPubsubMsgcontainer.property.msg_offset_idx_size = {}
function GucefPubsubMsgcontainer.property.msg_offset_idx_size:get()
  if self._m_msg_offset_idx_size ~= nil then
    return self._m_msg_offset_idx_size
  end

  local _pos = self._io:pos()
  self._io:seek((self._io:size() - 14))
  self._m_msg_offset_idx_size = self._io:read_u4le()
  self._io:seek(_pos)
  return self._m_msg_offset_idx_size
end


GucefPubsubMsgcontainer.GucefPubsubMsg = class.class(KaitaiStruct)

function GucefPubsubMsgcontainer.GucefPubsubMsg:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefPubsubMsgcontainer.GucefPubsubMsg:_read()
  if (self._root.header.options & (1 << 1)) ~= 0 then
    local _on = (self._root.header.options & (1 << 2))
    if _on == 0 then
      self.msg_date_time = self._io:read_u4le()
    else
      self.msg_date_time = self._io:read_u8le()
    end
  end
  if (self._root.header.options & (1 << 3)) ~= 0 then
    self.msgid = GucefCoreVariant(self._io)
  end
  if (self._root.header.options & (1 << 4)) ~= 0 then
    self.msg_index = GucefCoreVariant(self._io)
  end
  if (self._root.header.options & (1 << 5)) ~= 0 then
    self.primary_payload_included = GucefCoreVariant(self._io)
  end
  if (self._root.header.options & (1 << 6)) ~= 0 then
    self.nr_of_kv_pairs = self._io:read_u4le()
  end
  if (self._root.header.options & (1 << 6)) ~= 0 then
    self.key_value_pairs = {}
    for i = 0, self.nr_of_kv_pairs - 1 do
      self.key_value_pairs[i + 1] = GucefPubsubMsgcontainer.KvPair(self._io, self, self._root)
    end
  end
  if (self._root.header.options & (1 << 7)) ~= 0 then
    self.nr_of_metadata_kv_pairs = self._io:read_u4le()
  end
  if (self._root.header.options & (1 << 7)) ~= 0 then
    self.metadata_key_value_pairs = {}
    for i = 0, self.nr_of_metadata_kv_pairs - 1 do
      self.metadata_key_value_pairs[i + 1] = GucefPubsubMsgcontainer.KvPair(self._io, self, self._root)
    end
  end
end


GucefPubsubMsgcontainer.KvPair = class.class(KaitaiStruct)

function GucefPubsubMsgcontainer.KvPair:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefPubsubMsgcontainer.KvPair:_read()
  self.key = GucefCoreVariant(self._io)
  self.value = GucefCoreVariant(self._io)
end


