// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

using Kaitai;
using System.Collections.Generic;

namespace GUCEF
{
    public partial class GucefPubsubMsgcontainerFooter : KaitaiStruct
    {
        public static GucefPubsubMsgcontainerFooter FromFile(string fileName)
        {
            return new GucefPubsubMsgcontainerFooter(new KaitaiStream(fileName));
        }

        public GucefPubsubMsgcontainerFooter(KaitaiStream p__io, KaitaiStruct p__parent = null, GucefPubsubMsgcontainerFooter p__root = null) : base(p__io)
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
            f_mois = false;
            _read();
        }
        private void _read()
        {
            _msgOffsetIdx = new List<uint>();
            for (var i = 0; i < Mois; i++)
            {
                _msgOffsetIdx.Add(m_io.ReadU4le());
            }
            _msgOffsetIdxSize = m_io.ReadU4le();
            _magicText = m_io.ReadBytes(10);
            if (!((KaitaiStream.ByteArrayCompare(MagicText, new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }) == 0)))
            {
                throw new ValidationNotEqualError(new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }, MagicText, M_Io, "/seq/2");
            }
        }
        private bool f_mois;
        private uint _mois;
        public uint Mois
        {
            get
            {
                if (f_mois)
                    return _mois;
                long _pos = m_io.Pos;
                m_io.Seek((M_Io.Size - 14));
                _mois = m_io.ReadU4le();
                m_io.Seek(_pos);
                f_mois = true;
                return _mois;
            }
        }
        private List<uint> _msgOffsetIdx;
        private uint _msgOffsetIdxSize;
        private byte[] _magicText;
        private GucefPubsubMsgcontainerFooter m_root;
        private KaitaiStruct m_parent;
        public List<uint> MsgOffsetIdx { get { return _msgOffsetIdx; } }
        public uint MsgOffsetIdxSize { get { return _msgOffsetIdxSize; } }
        public byte[] MagicText { get { return _magicText; } }
        public GucefPubsubMsgcontainerFooter M_Root { get { return m_root; } }
        public KaitaiStruct M_Parent { get { return m_parent; } }
    }
}
