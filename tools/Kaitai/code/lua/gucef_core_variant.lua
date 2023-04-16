-- This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild
--
-- This file is compatible with Lua 5.3

local class = require("class")
require("kaitaistruct")
local enum = require("enum")

require("gucef_core_dynamic")
GucefCoreVariant = class.class(KaitaiStruct)

GucefCoreVariant.GucefDatatypes = enum.Enum {
  uint8 = 1,
  int8 = 2,
  uint16 = 3,
  int16 = 5,
  uint32 = 7,
  int32 = 9,
  uint64 = 11,
  int64 = 13,
  float32 = 15,
  float64 = 17,
  ascii_string = 21,
  utf8_string = 22,
  utf16_le_string = 23,
  utf16_be_string = 24,
  utf32_string = 25,
  boolean_int32 = 30,
  boolean_ascii_string = 31,
  boolean_utf8_string = 32,
  binary_blob = 40,
  binary_bsob = 41,
  datetime_iso8601_ascii_string = 50,
  datetime_iso8601_utf8_string = 51,
  le_timestamp_in_secs_since_unix_epoch = 55,
  be_timestamp_in_secs_since_unix_epoch = 56,
  le_timestamp_in_ms_since_unix_epoch = 57,
  be_timestamp_in_ms_since_unix_epoch = 58,
}

function GucefCoreVariant:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant:_read()
  self.type_id = GucefCoreVariant.GucefDatatypes(self._io:read_u1())
  local _on = self.type_id
  if _on == GucefCoreVariant.GucefDatatypes.datetime_iso8601_ascii_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.utf16_le_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.int16 then
    self.payload = GucefCoreVariant.GucefInt16(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.float32 then
    self.payload = GucefCoreVariant.GucefFloat32(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.ascii_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.uint16 then
    self.payload = GucefCoreVariant.GucefUint16(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.uint64 then
    self.payload = GucefCoreVariant.GucefUint64(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.int32 then
    self.payload = GucefCoreVariant.GucefInt32(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.uint8 then
    self.payload = GucefCoreVariant.GucefUint8(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.uint32 then
    self.payload = GucefCoreVariant.GucefUint32(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.float64 then
    self.payload = GucefCoreVariant.GucefFloat64(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.be_timestamp_in_secs_since_unix_epoch then
    self.payload = GucefCoreVariant.GucefBeTimestampInSecsSinceUnixEpoch(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.be_timestamp_in_ms_since_unix_epoch then
    self.payload = GucefCoreVariant.GucefBeTimestampInMsSinceUnixEpoch(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.utf32_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.boolean_utf8_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.binary_blob then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.le_timestamp_in_ms_since_unix_epoch then
    self.payload = GucefCoreVariant.GucefLeTimestampInMsSinceUnixEpoch(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.binary_bsob then
    self.payload = GucefCoreVariant.GucefBsob(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.boolean_ascii_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.utf8_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.int8 then
    self.payload = GucefCoreVariant.GucefInt8(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.boolean_int32 then
    self.payload = GucefCoreVariant.GucefBooleanInt32(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.le_timestamp_in_secs_since_unix_epoch then
    self.payload = GucefCoreVariant.GucefLeTimestampInSecsSinceUnixEpoch(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.datetime_iso8601_utf8_string then
    self.payload = GucefCoreDynamic(self._io)
  elseif _on == GucefCoreVariant.GucefDatatypes.int64 then
    self.payload = GucefCoreVariant.GucefInt64(self._io, self, self._root)
  elseif _on == GucefCoreVariant.GucefDatatypes.utf16_be_string then
    self.payload = GucefCoreDynamic(self._io)
  end
end


GucefCoreVariant.GucefInt64 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefInt64:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefInt64:_read()
  self.data = self._io:read_s8le()
end


GucefCoreVariant.GucefBsob = class.class(KaitaiStruct)

function GucefCoreVariant.GucefBsob:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefBsob:_read()
  self.data = self._io:read_u1()
end


GucefCoreVariant.GucefBeTimestampInMsSinceUnixEpoch = class.class(KaitaiStruct)

function GucefCoreVariant.GucefBeTimestampInMsSinceUnixEpoch:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefBeTimestampInMsSinceUnixEpoch:_read()
  self.data = self._io:read_u8le()
end


GucefCoreVariant.GucefLeTimestampInSecsSinceUnixEpoch = class.class(KaitaiStruct)

function GucefCoreVariant.GucefLeTimestampInSecsSinceUnixEpoch:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefLeTimestampInSecsSinceUnixEpoch:_read()
  self.data = self._io:read_u8le()
end


GucefCoreVariant.GucefUint16 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefUint16:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefUint16:_read()
  self.data = self._io:read_u2le()
end


GucefCoreVariant.GucefUint64 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefUint64:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefUint64:_read()
  self.data = self._io:read_u8le()
end


GucefCoreVariant.GucefUint8 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefUint8:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefUint8:_read()
  self.data = self._io:read_u1()
end


GucefCoreVariant.GucefInt8 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefInt8:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefInt8:_read()
  self.data = self._io:read_s1()
end


GucefCoreVariant.GucefInt16 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefInt16:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefInt16:_read()
  self.data = self._io:read_s2le()
end


GucefCoreVariant.GucefFloat64 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefFloat64:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefFloat64:_read()
  self.data = self._io:read_f8le()
end


GucefCoreVariant.GucefLeTimestampInMsSinceUnixEpoch = class.class(KaitaiStruct)

function GucefCoreVariant.GucefLeTimestampInMsSinceUnixEpoch:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefLeTimestampInMsSinceUnixEpoch:_read()
  self.data = self._io:read_u8le()
end


GucefCoreVariant.GucefInt32 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefInt32:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefInt32:_read()
  self.data = self._io:read_s4le()
end


GucefCoreVariant.GucefBeTimestampInSecsSinceUnixEpoch = class.class(KaitaiStruct)

function GucefCoreVariant.GucefBeTimestampInSecsSinceUnixEpoch:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefBeTimestampInSecsSinceUnixEpoch:_read()
  self.data = self._io:read_u8le()
end


GucefCoreVariant.GucefFloat32 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefFloat32:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefFloat32:_read()
  self.data = self._io:read_f4le()
end


GucefCoreVariant.GucefUint32 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefUint32:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefUint32:_read()
  self.data = self._io:read_u4le()
end


GucefCoreVariant.GucefBooleanInt32 = class.class(KaitaiStruct)

function GucefCoreVariant.GucefBooleanInt32:_init(io, parent, root)
  KaitaiStruct._init(self, io)
  self._parent = parent
  self._root = root or self
  self:_read()
end

function GucefCoreVariant.GucefBooleanInt32:_read()
  self.data = self._io:read_s4le()
end


