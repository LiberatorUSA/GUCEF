// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

using Kaitai;
using System.Collections.Generic;

namespace GUCEF
{
    public partial class GucefPubsubMsgcontainer : KaitaiStruct
    {
        public static GucefPubsubMsgcontainer FromFile(string fileName)
        {
            return new GucefPubsubMsgcontainer(new KaitaiStream(fileName));
        }

        public GucefPubsubMsgcontainer(KaitaiStream p__io, KaitaiStruct p__parent = null, GucefPubsubMsgcontainer p__root = null) : base(p__io)
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
            f_msgOffsetIdxSize = false;
            _read();
        }
        private void _read()
        {
            _header = new GucefPubsubMsgcontainerHeader(m_io);
            _pubsubmsg = new List<GucefPubsubMsg>();
            {
                var i = 0;
                GucefPubsubMsg M_;
                do {
                    M_ = new GucefPubsubMsg(m_io, this, m_root);
                    _pubsubmsg.Add(M_);
                    i++;
                } while (!(M_Io.Pos == (((M_Io.Size - (MsgOffsetIdxSize * 4)) - 4) - 10)));
            }
            _footer = new GucefPubsubMsgcontainerFooter(m_io);
        }
        public partial class GucefPubsubMsg : KaitaiStruct
        {
            public static GucefPubsubMsg FromFile(string fileName)
            {
                return new GucefPubsubMsg(new KaitaiStream(fileName));
            }

            public GucefPubsubMsg(KaitaiStream p__io, GucefPubsubMsgcontainer p__parent = null, GucefPubsubMsgcontainer p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                if ((M_Root.Header.Options & (1 << 1)) != 0) {
                    switch ((M_Root.Header.Options & (1 << 2))) {
                    case 0: {
                        _msgDateTime = m_io.ReadU4le();
                        break;
                    }
                    default: {
                        _msgDateTime = m_io.ReadU8le();
                        break;
                    }
                    }
                }
                if ((M_Root.Header.Options & (1 << 3)) != 0) {
                    _msgid = new GucefCoreVariant(m_io);
                }
                if ((M_Root.Header.Options & (1 << 4)) != 0) {
                    _msgIndex = new GucefCoreVariant(m_io);
                }
                if ((M_Root.Header.Options & (1 << 5)) != 0) {
                    _primaryPayloadIncluded = new GucefCoreVariant(m_io);
                }
                if ((M_Root.Header.Options & (1 << 6)) != 0) {
                    _nrOfKvPairs = m_io.ReadU4le();
                }
                if ((M_Root.Header.Options & (1 << 6)) != 0) {
                    _keyValuePairs = new List<KvPair>();
                    for (var i = 0; i < NrOfKvPairs; i++)
                    {
                        _keyValuePairs.Add(new KvPair(m_io, this, m_root));
                    }
                }
                if ((M_Root.Header.Options & (1 << 7)) != 0) {
                    _nrOfMetadataKvPairs = m_io.ReadU4le();
                }
                if ((M_Root.Header.Options & (1 << 7)) != 0) {
                    _metadataKeyValuePairs = new List<KvPair>();
                    for (var i = 0; i < NrOfMetadataKvPairs; i++)
                    {
                        _metadataKeyValuePairs.Add(new KvPair(m_io, this, m_root));
                    }
                }
            }
            private ulong _msgDateTime;
            private GucefCoreVariant _msgid;
            private GucefCoreVariant _msgIndex;
            private GucefCoreVariant _primaryPayloadIncluded;
            private uint? _nrOfKvPairs;
            private List<KvPair> _keyValuePairs;
            private uint? _nrOfMetadataKvPairs;
            private List<KvPair> _metadataKeyValuePairs;
            private GucefPubsubMsgcontainer m_root;
            private GucefPubsubMsgcontainer m_parent;
            public ulong MsgDateTime { get { return _msgDateTime; } }
            public GucefCoreVariant Msgid { get { return _msgid; } }
            public GucefCoreVariant MsgIndex { get { return _msgIndex; } }
            public GucefCoreVariant PrimaryPayloadIncluded { get { return _primaryPayloadIncluded; } }
            public uint? NrOfKvPairs { get { return _nrOfKvPairs; } }
            public List<KvPair> KeyValuePairs { get { return _keyValuePairs; } }
            public uint? NrOfMetadataKvPairs { get { return _nrOfMetadataKvPairs; } }
            public List<KvPair> MetadataKeyValuePairs { get { return _metadataKeyValuePairs; } }
            public GucefPubsubMsgcontainer M_Root { get { return m_root; } }
            public GucefPubsubMsgcontainer M_Parent { get { return m_parent; } }
        }
        public partial class KvPair : KaitaiStruct
        {
            public static KvPair FromFile(string fileName)
            {
                return new KvPair(new KaitaiStream(fileName));
            }

            public KvPair(KaitaiStream p__io, GucefPubsubMsgcontainer.GucefPubsubMsg p__parent = null, GucefPubsubMsgcontainer p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _key = new GucefCoreVariant(m_io);
                _value = new GucefCoreVariant(m_io);
            }
            private GucefCoreVariant _key;
            private GucefCoreVariant _value;
            private GucefPubsubMsgcontainer m_root;
            private GucefPubsubMsgcontainer.GucefPubsubMsg m_parent;
            public GucefCoreVariant Key { get { return _key; } }
            public GucefCoreVariant Value { get { return _value; } }
            public GucefPubsubMsgcontainer M_Root { get { return m_root; } }
            public GucefPubsubMsgcontainer.GucefPubsubMsg M_Parent { get { return m_parent; } }
        }
        private bool f_msgOffsetIdxSize;
        private uint _msgOffsetIdxSize;
        public uint MsgOffsetIdxSize
        {
            get
            {
                if (f_msgOffsetIdxSize)
                    return _msgOffsetIdxSize;
                long _pos = m_io.Pos;
                m_io.Seek((M_Io.Size - 14));
                _msgOffsetIdxSize = m_io.ReadU4le();
                m_io.Seek(_pos);
                f_msgOffsetIdxSize = true;
                return _msgOffsetIdxSize;
            }
        }
        private GucefPubsubMsgcontainerHeader _header;
        private List<GucefPubsubMsg> _pubsubmsg;
        private GucefPubsubMsgcontainerFooter _footer;
        private GucefPubsubMsgcontainer m_root;
        private KaitaiStruct m_parent;
        public GucefPubsubMsgcontainerHeader Header { get { return _header; } }
        public List<GucefPubsubMsg> Pubsubmsg { get { return _pubsubmsg; } }
        public GucefPubsubMsgcontainerFooter Footer { get { return _footer; } }
        public GucefPubsubMsgcontainer M_Root { get { return m_root; } }
        public KaitaiStruct M_Parent { get { return m_parent; } }
    }
}
