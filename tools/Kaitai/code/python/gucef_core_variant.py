# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import kaitaistruct
from kaitaistruct import KaitaiStruct, KaitaiStream, BytesIO
from enum import Enum


if getattr(kaitaistruct, 'API_VERSION', (0, 9)) < (0, 9):
    raise Exception("Incompatible Kaitai Struct Python API: 0.9 or later is required, but you have %s" % (kaitaistruct.__version__))

import gucef_core_dynamic
class GucefCoreVariant(KaitaiStruct):

    class GucefDatatypes(Enum):
        uint8 = 1
        int8 = 2
        uint16 = 3
        int16 = 5
        uint32 = 7
        int32 = 9
        uint64 = 11
        int64 = 13
        float32 = 15
        float64 = 17
        ascii_string = 21
        utf8_string = 22
        utf16_le_string = 23
        utf16_be_string = 24
        utf32_string = 25
        boolean_int32 = 30
        boolean_ascii_string = 31
        boolean_utf8_string = 32
        binary_blob = 40
        binary_bsob = 41
        datetime_iso8601_ascii_string = 50
        datetime_iso8601_utf8_string = 51
        le_timestamp_in_secs_since_unix_epoch = 55
        be_timestamp_in_secs_since_unix_epoch = 56
        le_timestamp_in_ms_since_unix_epoch = 57
        be_timestamp_in_ms_since_unix_epoch = 58
    def __init__(self, _io, _parent=None, _root=None):
        self._io = _io
        self._parent = _parent
        self._root = _root if _root else self
        self._read()

    def _read(self):
        self.type_id = KaitaiStream.resolve_enum(GucefCoreVariant.GucefDatatypes, self._io.read_u1())
        _on = self.type_id
        if _on == GucefCoreVariant.GucefDatatypes.datetime_iso8601_ascii_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.utf16_le_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.int16:
            self.payload = GucefCoreVariant.GucefInt16(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.float32:
            self.payload = GucefCoreVariant.GucefFloat32(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.ascii_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.uint16:
            self.payload = GucefCoreVariant.GucefUint16(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.uint64:
            self.payload = GucefCoreVariant.GucefUint64(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.int32:
            self.payload = GucefCoreVariant.GucefInt32(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.uint8:
            self.payload = GucefCoreVariant.GucefUint8(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.uint32:
            self.payload = GucefCoreVariant.GucefUint32(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.float64:
            self.payload = GucefCoreVariant.GucefFloat64(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.be_timestamp_in_secs_since_unix_epoch:
            self.payload = GucefCoreVariant.GucefBeTimestampInSecsSinceUnixEpoch(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.be_timestamp_in_ms_since_unix_epoch:
            self.payload = GucefCoreVariant.GucefBeTimestampInMsSinceUnixEpoch(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.utf32_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.boolean_utf8_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.binary_blob:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.le_timestamp_in_ms_since_unix_epoch:
            self.payload = GucefCoreVariant.GucefLeTimestampInMsSinceUnixEpoch(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.binary_bsob:
            self.payload = GucefCoreVariant.GucefBsob(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.boolean_ascii_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.utf8_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.int8:
            self.payload = GucefCoreVariant.GucefInt8(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.boolean_int32:
            self.payload = GucefCoreVariant.GucefBooleanInt32(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.le_timestamp_in_secs_since_unix_epoch:
            self.payload = GucefCoreVariant.GucefLeTimestampInSecsSinceUnixEpoch(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.datetime_iso8601_utf8_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)
        elif _on == GucefCoreVariant.GucefDatatypes.int64:
            self.payload = GucefCoreVariant.GucefInt64(self._io, self, self._root)
        elif _on == GucefCoreVariant.GucefDatatypes.utf16_be_string:
            self.payload = gucef_core_dynamic.GucefCoreDynamic(self._io)

    class GucefInt64(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_s8le()


    class GucefBsob(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u1()


    class GucefBeTimestampInMsSinceUnixEpoch(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u8le()


    class GucefLeTimestampInSecsSinceUnixEpoch(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u8le()


    class GucefUint16(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u2le()


    class GucefUint64(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u8le()


    class GucefUint8(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u1()


    class GucefInt8(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_s1()


    class GucefInt16(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_s2le()


    class GucefFloat64(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_f8le()


    class GucefLeTimestampInMsSinceUnixEpoch(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u8le()


    class GucefInt32(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_s4le()


    class GucefBeTimestampInSecsSinceUnixEpoch(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u8le()


    class GucefFloat32(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_f4le()


    class GucefUint32(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_u4le()


    class GucefBooleanInt32(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.data = self._io.read_s4le()



