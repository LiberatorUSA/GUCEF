// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

using Kaitai;

namespace GUCEF
{
    public partial class GucefPubsubMsgcontainerHeader : KaitaiStruct
    {
        public static GucefPubsubMsgcontainerHeader FromFile(string fileName)
        {
            return new GucefPubsubMsgcontainerHeader(new KaitaiStream(fileName));
        }

        public GucefPubsubMsgcontainerHeader(KaitaiStream p__io, KaitaiStruct p__parent = null, GucefPubsubMsgcontainerHeader p__root = null) : base(p__io)
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
            _read();
        }
        private void _read()
        {
            _magicText = m_io.ReadBytes(10);
            if (!((KaitaiStream.ByteArrayCompare(MagicText, new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }) == 0)))
            {
                throw new ValidationNotEqualError(new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }, MagicText, M_Io, "/seq/0");
            }
            _version = m_io.ReadU1();
            _options = m_io.ReadU4le();
        }
        private byte[] _magicText;
        private byte _version;
        private uint _options;
        private GucefPubsubMsgcontainerHeader m_root;
        private KaitaiStruct m_parent;
        public byte[] MagicText { get { return _magicText; } }
        public byte Version { get { return _version; } }
        public uint Options { get { return _options; } }
        public GucefPubsubMsgcontainerHeader M_Root { get { return m_root; } }
        public KaitaiStruct M_Parent { get { return m_parent; } }
    }
}
