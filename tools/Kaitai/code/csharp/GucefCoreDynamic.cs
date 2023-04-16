// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

using Kaitai;

namespace GUCEF
{
    public partial class GucefCoreDynamic : KaitaiStruct
    {
        public static GucefCoreDynamic FromFile(string fileName)
        {
            return new GucefCoreDynamic(new KaitaiStream(fileName));
        }

        public GucefCoreDynamic(KaitaiStream p__io, KaitaiStruct p__parent = null, GucefCoreDynamic p__root = null) : base(p__io)
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
            _read();
        }
        private void _read()
        {
            _payloadSize = m_io.ReadU4le();
            _data = m_io.ReadBytes(PayloadSize);
        }
        private uint _payloadSize;
        private byte[] _data;
        private GucefCoreDynamic m_root;
        private KaitaiStruct m_parent;
        public uint PayloadSize { get { return _payloadSize; } }
        public byte[] Data { get { return _data; } }
        public GucefCoreDynamic M_Root { get { return m_root; } }
        public KaitaiStruct M_Parent { get { return m_parent; } }
    }
}
