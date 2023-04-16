-- This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild
--
-- This file is compatible with Lua 5.3

local class = require("class")
require("kaitaistruct")

GucefPubsubMsgcontainerHeader = class.class(KaitaiStruct)

function GucefPubsubMsgcontainerHeader:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefPubsubMsgcontainerHeader:_read()
  self.magic_text = self._io:read_bytes(10)
  if not(self.magic_text == "\080\085\066\083\085\066\077\083\071\083") then
    error("not equal, expected " ..  "\080\085\066\083\085\066\077\083\071\083" .. ", but got " .. self.magic_text)
  end
  self.version = self._io:read_u1()
  self.options = self._io:read_u4le()
end


