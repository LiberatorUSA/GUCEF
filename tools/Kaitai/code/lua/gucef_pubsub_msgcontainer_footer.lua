-- This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild
--
-- This file is compatible with Lua 5.3

local class = require("class")
require("kaitaistruct")

GucefPubsubMsgcontainerFooter = class.class(KaitaiStruct)

function GucefPubsubMsgcontainerFooter:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefPubsubMsgcontainerFooter:_read()
  self.msg_offset_idx = {}
  for i = 0, self.mois - 1 do
    self.msg_offset_idx[i + 1] = self._io:read_u4le()
  end
  self.msg_offset_idx_size = self._io:read_u4le()
  self.magic_text = self._io:read_bytes(10)
  if not(self.magic_text == "\080\085\066\083\085\066\077\083\071\083") then
    error("not equal, expected " ..  "\080\085\066\083\085\066\077\083\071\083" .. ", but got " .. self.magic_text)
  end
end

GucefPubsubMsgcontainerFooter.property.mois = {}
function GucefPubsubMsgcontainerFooter.property.mois:get()
  if self._m_mois ~= nil then
    return self._m_mois
  end

  local _pos = self._io:pos()
  self._io:seek((self._io:size() - 14))
  self._m_mois = self._io:read_u4le()
  self._io:seek(_pos)
  return self._m_mois
end


