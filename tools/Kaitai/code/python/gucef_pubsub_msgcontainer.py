# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import kaitaistruct
from kaitaistruct import KaitaiStruct, KaitaiStream, BytesIO


if getattr(kaitaistruct, 'API_VERSION', (0, 9)) < (0, 9):
    raise Exception("Incompatible Kaitai Struct Python API: 0.9 or later is required, but you have %s" % (kaitaistruct.__version__))

import gucef_pubsub_msgcontainer_footer
import gucef_core_variant
import gucef_pubsub_msgcontainer_header
class GucefPubsubMsgcontainer(KaitaiStruct):
    def __init__(self, _io, _parent=None, _root=None):
        self._io = _io
        self._parent = _parent
        self._root = _root if _root else self
        self._read()

    def _read(self):
        self.header = gucef_pubsub_msgcontainer_header.GucefPubsubMsgcontainerHeader(self._io)
        self.pubsubmsg = []
        i = 0
        while True:
            _ = GucefPubsubMsgcontainer.GucefPubsubMsg(self._io, self, self._root)
            self.pubsubmsg.append(_)
            if self._io.pos() == (((self._io.size() - (self.msg_offset_idx_size * 4)) - 4) - 10):
                break
            i += 1
        self.footer = gucef_pubsub_msgcontainer_footer.GucefPubsubMsgcontainerFooter(self._io)

    class GucefPubsubMsg(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            if (self._root.header.options & (1 << 1)) != 0:
                _on = (self._root.header.options & (1 << 2))
                if _on == 0:
                    self.msg_date_time = self._io.read_u4le()
                else:
                    self.msg_date_time = self._io.read_u8le()

            if (self._root.header.options & (1 << 3)) != 0:
                self.msgid = gucef_core_variant.GucefCoreVariant(self._io)

            if (self._root.header.options & (1 << 4)) != 0:
                self.msg_index = gucef_core_variant.GucefCoreVariant(self._io)

            if (self._root.header.options & (1 << 5)) != 0:
                self.primary_payload_included = gucef_core_variant.GucefCoreVariant(self._io)

            if (self._root.header.options & (1 << 6)) != 0:
                self.nr_of_kv_pairs = self._io.read_u4le()

            if (self._root.header.options & (1 << 6)) != 0:
                self.key_value_pairs = []
                for i in range(self.nr_of_kv_pairs):
                    self.key_value_pairs.append(GucefPubsubMsgcontainer.KvPair(self._io, self, self._root))


            if (self._root.header.options & (1 << 7)) != 0:
                self.nr_of_metadata_kv_pairs = self._io.read_u4le()

            if (self._root.header.options & (1 << 7)) != 0:
                self.metadata_key_value_pairs = []
                for i in range(self.nr_of_metadata_kv_pairs):
                    self.metadata_key_value_pairs.append(GucefPubsubMsgcontainer.KvPair(self._io, self, self._root))




    class KvPair(KaitaiStruct):
        def __init__(self, _io, _parent=None, _root=None):
            self._io = _io
            self._parent = _parent
            self._root = _root if _root else self
            self._read()

        def _read(self):
            self.key = gucef_core_variant.GucefCoreVariant(self._io)
            self.value = gucef_core_variant.GucefCoreVariant(self._io)


    @property
    def msg_offset_idx_size(self):
        if hasattr(self, '_m_msg_offset_idx_size'):
            return self._m_msg_offset_idx_size

        _pos = self._io.pos()
        self._io.seek((self._io.size() - 14))
        self._m_msg_offset_idx_size = self._io.read_u4le()
        self._io.seek(_pos)
        return getattr(self, '_m_msg_offset_idx_size', None)


