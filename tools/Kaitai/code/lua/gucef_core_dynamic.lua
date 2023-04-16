-- This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild
--
-- This file is compatible with Lua 5.3

local class = require("class")
require("kaitaistruct")

GucefCoreDynamic = class.class(KaitaiStruct)

function GucefCoreDynamic:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreDynamic:_read()
  self.payload_size = self._io:read_u4le()
  self.data = self._io:read_bytes(self.payload_size)
end


