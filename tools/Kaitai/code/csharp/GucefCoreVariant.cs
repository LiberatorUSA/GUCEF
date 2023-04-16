// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

using Kaitai;

namespace GUCEF
{
    public partial class GucefCoreVariant : KaitaiStruct
    {
        public static GucefCoreVariant FromFile(string fileName)
        {
            return new GucefCoreVariant(new KaitaiStream(fileName));
        }


        public enum GucefDatatypes
        {
            Uint8 = 1,
            Int8 = 2,
            Uint16 = 3,
            Int16 = 5,
            Uint32 = 7,
            Int32 = 9,
            Uint64 = 11,
            Int64 = 13,
            Float32 = 15,
            Float64 = 17,
            AsciiString = 21,
            Utf8String = 22,
            Utf16LeString = 23,
            Utf16BeString = 24,
            Utf32String = 25,
            BooleanInt32 = 30,
            BooleanAsciiString = 31,
            BooleanUtf8String = 32,
            BinaryBlob = 40,
            BinaryBsob = 41,
            DatetimeIso8601AsciiString = 50,
            DatetimeIso8601Utf8String = 51,
            LeTimestampInSecsSinceUnixEpoch = 55,
            BeTimestampInSecsSinceUnixEpoch = 56,
            LeTimestampInMsSinceUnixEpoch = 57,
            BeTimestampInMsSinceUnixEpoch = 58,
        }
        public GucefCoreVariant(KaitaiStream p__io, KaitaiStruct p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
            _read();
        }
        private void _read()
        {
            _typeId = ((GucefDatatypes) m_io.ReadU1());
            switch (TypeId) {
            case GucefDatatypes.DatetimeIso8601AsciiString: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.Utf16LeString: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.Int16: {
                _payload = new GucefInt16(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Float32: {
                _payload = new GucefFloat32(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.AsciiString: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.Uint16: {
                _payload = new GucefUint16(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Uint64: {
                _payload = new GucefUint64(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Int32: {
                _payload = new GucefInt32(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Uint8: {
                _payload = new GucefUint8(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Uint32: {
                _payload = new GucefUint32(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Float64: {
                _payload = new GucefFloat64(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.BeTimestampInSecsSinceUnixEpoch: {
                _payload = new GucefBeTimestampInSecsSinceUnixEpoch(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.BeTimestampInMsSinceUnixEpoch: {
                _payload = new GucefBeTimestampInMsSinceUnixEpoch(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Utf32String: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.BooleanUtf8String: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.BinaryBlob: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.LeTimestampInMsSinceUnixEpoch: {
                _payload = new GucefLeTimestampInMsSinceUnixEpoch(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.BinaryBsob: {
                _payload = new GucefBsob(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.BooleanAsciiString: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.Utf8String: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.Int8: {
                _payload = new GucefInt8(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.BooleanInt32: {
                _payload = new GucefBooleanInt32(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.LeTimestampInSecsSinceUnixEpoch: {
                _payload = new GucefLeTimestampInSecsSinceUnixEpoch(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.DatetimeIso8601Utf8String: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            case GucefDatatypes.Int64: {
                _payload = new GucefInt64(m_io, this, m_root);
                break;
            }
            case GucefDatatypes.Utf16BeString: {
                _payload = new GucefCoreDynamic(m_io);
                break;
            }
            }
        }
        public partial class GucefInt64 : KaitaiStruct
        {
            public static GucefInt64 FromFile(string fileName)
            {
                return new GucefInt64(new KaitaiStream(fileName));
            }

            public GucefInt64(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadS8le();
            }
            private long _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public long Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefBsob : KaitaiStruct
        {
            public static GucefBsob FromFile(string fileName)
            {
                return new GucefBsob(new KaitaiStream(fileName));
            }

            public GucefBsob(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU1();
            }
            private byte _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public byte Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefBeTimestampInMsSinceUnixEpoch : KaitaiStruct
        {
            public static GucefBeTimestampInMsSinceUnixEpoch FromFile(string fileName)
            {
                return new GucefBeTimestampInMsSinceUnixEpoch(new KaitaiStream(fileName));
            }

            public GucefBeTimestampInMsSinceUnixEpoch(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU8le();
            }
            private ulong _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public ulong Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefLeTimestampInSecsSinceUnixEpoch : KaitaiStruct
        {
            public static GucefLeTimestampInSecsSinceUnixEpoch FromFile(string fileName)
            {
                return new GucefLeTimestampInSecsSinceUnixEpoch(new KaitaiStream(fileName));
            }

            public GucefLeTimestampInSecsSinceUnixEpoch(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU8le();
            }
            private ulong _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public ulong Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefUint16 : KaitaiStruct
        {
            public static GucefUint16 FromFile(string fileName)
            {
                return new GucefUint16(new KaitaiStream(fileName));
            }

            public GucefUint16(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU2le();
            }
            private ushort _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public ushort Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefUint64 : KaitaiStruct
        {
            public static GucefUint64 FromFile(string fileName)
            {
                return new GucefUint64(new KaitaiStream(fileName));
            }

            public GucefUint64(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU8le();
            }
            private ulong _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public ulong Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefUint8 : KaitaiStruct
        {
            public static GucefUint8 FromFile(string fileName)
            {
                return new GucefUint8(new KaitaiStream(fileName));
            }

            public GucefUint8(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU1();
            }
            private byte _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public byte Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefInt8 : KaitaiStruct
        {
            public static GucefInt8 FromFile(string fileName)
            {
                return new GucefInt8(new KaitaiStream(fileName));
            }

            public GucefInt8(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadS1();
            }
            private sbyte _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public sbyte Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefInt16 : KaitaiStruct
        {
            public static GucefInt16 FromFile(string fileName)
            {
                return new GucefInt16(new KaitaiStream(fileName));
            }

            public GucefInt16(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadS2le();
            }
            private short _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public short Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefFloat64 : KaitaiStruct
        {
            public static GucefFloat64 FromFile(string fileName)
            {
                return new GucefFloat64(new KaitaiStream(fileName));
            }

            public GucefFloat64(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadF8le();
            }
            private double _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public double Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefLeTimestampInMsSinceUnixEpoch : KaitaiStruct
        {
            public static GucefLeTimestampInMsSinceUnixEpoch FromFile(string fileName)
            {
                return new GucefLeTimestampInMsSinceUnixEpoch(new KaitaiStream(fileName));
            }

            public GucefLeTimestampInMsSinceUnixEpoch(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU8le();
            }
            private ulong _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public ulong Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefInt32 : KaitaiStruct
        {
            public static GucefInt32 FromFile(string fileName)
            {
                return new GucefInt32(new KaitaiStream(fileName));
            }

            public GucefInt32(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadS4le();
            }
            private int _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public int Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefBeTimestampInSecsSinceUnixEpoch : KaitaiStruct
        {
            public static GucefBeTimestampInSecsSinceUnixEpoch FromFile(string fileName)
            {
                return new GucefBeTimestampInSecsSinceUnixEpoch(new KaitaiStream(fileName));
            }

            public GucefBeTimestampInSecsSinceUnixEpoch(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU8le();
            }
            private ulong _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public ulong Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefFloat32 : KaitaiStruct
        {
            public static GucefFloat32 FromFile(string fileName)
            {
                return new GucefFloat32(new KaitaiStream(fileName));
            }

            public GucefFloat32(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadF4le();
            }
            private float _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public float Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefUint32 : KaitaiStruct
        {
            public static GucefUint32 FromFile(string fileName)
            {
                return new GucefUint32(new KaitaiStream(fileName));
            }

            public GucefUint32(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadU4le();
            }
            private uint _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public uint Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        public partial class GucefBooleanInt32 : KaitaiStruct
        {
            public static GucefBooleanInt32 FromFile(string fileName)
            {
                return new GucefBooleanInt32(new KaitaiStream(fileName));
            }

            public GucefBooleanInt32(KaitaiStream p__io, GucefCoreVariant p__parent = null, GucefCoreVariant p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _data = m_io.ReadS4le();
            }
            private int _data;
            private GucefCoreVariant m_root;
            private GucefCoreVariant m_parent;
            public int Data { get { return _data; } }
            public GucefCoreVariant M_Root { get { return m_root; } }
            public GucefCoreVariant M_Parent { get { return m_parent; } }
        }
        private GucefDatatypes _typeId;
        private KaitaiStruct _payload;
        private GucefCoreVariant m_root;
        private KaitaiStruct m_parent;
        public GucefDatatypes TypeId { get { return _typeId; } }
        public KaitaiStruct Payload { get { return _payload; } }
        public GucefCoreVariant M_Root { get { return m_root; } }
        public KaitaiStruct M_Parent { get { return m_parent; } }
    }
}
