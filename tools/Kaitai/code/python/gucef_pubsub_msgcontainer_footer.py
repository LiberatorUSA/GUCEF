# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import kaitaistruct
from kaitaistruct import KaitaiStruct, KaitaiStream, BytesIO


if getattr(kaitaistruct, 'API_VERSION', (0, 9)) < (0, 9):
    raise Exception("Incompatible Kaitai Struct Python API: 0.9 or later is required, but you have %s" % (kaitaistruct.__version__))

class GucefPubsubMsgcontainerFooter(KaitaiStruct):
    def __init__(self, _io, _parent=None, _root=None):
        self._io = _io
        self._parent = _parent
        self._root = _root if _root else self
        self._read()

    def _read(self):
        self.msg_offset_idx = []
        for i in range(self.mois):
            self.msg_offset_idx.append(self._io.read_u4le())

        self.msg_offset_idx_size = self._io.read_u4le()
        self.magic_text = self._io.read_bytes(10)
        if not self.magic_text == b"\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53":
            raise kaitaistruct.ValidationNotEqualError(b"\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53", self.magic_text, self._io, u"/seq/2")

    @property
    def mois(self):
        if hasattr(self, '_m_mois'):
            return self._m_mois

        _pos = self._io.pos()
        self._io.seek((self._io.size() - 14))
        self._m_mois = self._io.read_u4le()
        self._io.seek(_pos)
        return getattr(self, '_m_mois', None)


