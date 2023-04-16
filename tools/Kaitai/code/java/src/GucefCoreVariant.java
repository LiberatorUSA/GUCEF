// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import io.kaitai.struct.ByteBufferKaitaiStream;
import io.kaitai.struct.KaitaiStruct;
import io.kaitai.struct.KaitaiStream;
import java.io.IOException;
import java.util.Map;
import java.util.HashMap;

public class GucefCoreVariant extends KaitaiStruct {
    public static GucefCoreVariant fromFile(String fileName) throws IOException {
        return new GucefCoreVariant(new ByteBufferKaitaiStream(fileName));
    }

    public enum GucefDatatypes {
        UINT8(1),
        INT8(2),
        UINT16(3),
        INT16(5),
        UINT32(7),
        INT32(9),
        UINT64(11),
        INT64(13),
        FLOAT32(15),
        FLOAT64(17),
        ASCII_STRING(21),
        UTF8_STRING(22),
        UTF16_LE_STRING(23),
        UTF16_BE_STRING(24),
        UTF32_STRING(25),
        BOOLEAN_INT32(30),
        BOOLEAN_ASCII_STRING(31),
        BOOLEAN_UTF8_STRING(32),
        BINARY_BLOB(40),
        BINARY_BSOB(41),
        DATETIME_ISO8601_ASCII_STRING(50),
        DATETIME_ISO8601_UTF8_STRING(51),
        LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH(55),
        BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH(56),
        LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH(57),
        BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH(58);

        private final long id;
        GucefDatatypes(long id) { this.id = id; }
        public long id() { return id; }
        private static final Map<Long, GucefDatatypes> byId = new HashMap<Long, GucefDatatypes>(26);
        static {
            for (GucefDatatypes e : GucefDatatypes.values())
                byId.put(e.id(), e);
        }
        public static GucefDatatypes byId(long id) { return byId.get(id); }
    }

    public GucefCoreVariant(KaitaiStream _io) {
        this(_io, null, null);
    }

    public GucefCoreVariant(KaitaiStream _io, KaitaiStruct _parent) {
        this(_io, _parent, null);
    }

    public GucefCoreVariant(KaitaiStream _io, KaitaiStruct _parent, GucefCoreVariant _root) {
        super(_io);
        this._parent = _parent;
        this._root = _root == null ? this : _root;
        _read();
    }
    private void _read() {
        this.typeId = GucefDatatypes.byId(this._io.readU1());
        {
            GucefDatatypes on = typeId();
            if (on != null) {
                switch (typeId()) {
                case DATETIME_ISO8601_ASCII_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case UTF16_LE_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case INT16: {
                    this.payload = new GucefInt16(this._io, this, _root);
                    break;
                }
                case FLOAT32: {
                    this.payload = new GucefFloat32(this._io, this, _root);
                    break;
                }
                case ASCII_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case UINT16: {
                    this.payload = new GucefUint16(this._io, this, _root);
                    break;
                }
                case UINT64: {
                    this.payload = new GucefUint64(this._io, this, _root);
                    break;
                }
                case INT32: {
                    this.payload = new GucefInt32(this._io, this, _root);
                    break;
                }
                case UINT8: {
                    this.payload = new GucefUint8(this._io, this, _root);
                    break;
                }
                case UINT32: {
                    this.payload = new GucefUint32(this._io, this, _root);
                    break;
                }
                case FLOAT64: {
                    this.payload = new GucefFloat64(this._io, this, _root);
                    break;
                }
                case BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH: {
                    this.payload = new GucefBeTimestampInSecsSinceUnixEpoch(this._io, this, _root);
                    break;
                }
                case BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH: {
                    this.payload = new GucefBeTimestampInMsSinceUnixEpoch(this._io, this, _root);
                    break;
                }
                case UTF32_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case BOOLEAN_UTF8_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case BINARY_BLOB: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH: {
                    this.payload = new GucefLeTimestampInMsSinceUnixEpoch(this._io, this, _root);
                    break;
                }
                case BINARY_BSOB: {
                    this.payload = new GucefBsob(this._io, this, _root);
                    break;
                }
                case BOOLEAN_ASCII_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case UTF8_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case INT8: {
                    this.payload = new GucefInt8(this._io, this, _root);
                    break;
                }
                case BOOLEAN_INT32: {
                    this.payload = new GucefBooleanInt32(this._io, this, _root);
                    break;
                }
                case LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH: {
                    this.payload = new GucefLeTimestampInSecsSinceUnixEpoch(this._io, this, _root);
                    break;
                }
                case DATETIME_ISO8601_UTF8_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                case INT64: {
                    this.payload = new GucefInt64(this._io, this, _root);
                    break;
                }
                case UTF16_BE_STRING: {
                    this.payload = new GucefCoreDynamic(this._io);
                    break;
                }
                }
            }
        }
    }
    public static class GucefInt64 extends KaitaiStruct {
        public static GucefInt64 fromFile(String fileName) throws IOException {
            return new GucefInt64(new ByteBufferKaitaiStream(fileName));
        }

        public GucefInt64(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefInt64(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefInt64(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readS8le();
        }
        private long data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public long data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefBsob extends KaitaiStruct {
        public static GucefBsob fromFile(String fileName) throws IOException {
            return new GucefBsob(new ByteBufferKaitaiStream(fileName));
        }

        public GucefBsob(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefBsob(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefBsob(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU1();
        }
        private int data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public int data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefBeTimestampInMsSinceUnixEpoch extends KaitaiStruct {
        public static GucefBeTimestampInMsSinceUnixEpoch fromFile(String fileName) throws IOException {
            return new GucefBeTimestampInMsSinceUnixEpoch(new ByteBufferKaitaiStream(fileName));
        }

        public GucefBeTimestampInMsSinceUnixEpoch(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefBeTimestampInMsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefBeTimestampInMsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU8le();
        }
        private long data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public long data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefLeTimestampInSecsSinceUnixEpoch extends KaitaiStruct {
        public static GucefLeTimestampInSecsSinceUnixEpoch fromFile(String fileName) throws IOException {
            return new GucefLeTimestampInSecsSinceUnixEpoch(new ByteBufferKaitaiStream(fileName));
        }

        public GucefLeTimestampInSecsSinceUnixEpoch(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefLeTimestampInSecsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefLeTimestampInSecsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU8le();
        }
        private long data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public long data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefUint16 extends KaitaiStruct {
        public static GucefUint16 fromFile(String fileName) throws IOException {
            return new GucefUint16(new ByteBufferKaitaiStream(fileName));
        }

        public GucefUint16(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefUint16(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefUint16(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU2le();
        }
        private int data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public int data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefUint64 extends KaitaiStruct {
        public static GucefUint64 fromFile(String fileName) throws IOException {
            return new GucefUint64(new ByteBufferKaitaiStream(fileName));
        }

        public GucefUint64(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefUint64(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefUint64(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU8le();
        }
        private long data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public long data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefUint8 extends KaitaiStruct {
        public static GucefUint8 fromFile(String fileName) throws IOException {
            return new GucefUint8(new ByteBufferKaitaiStream(fileName));
        }

        public GucefUint8(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefUint8(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefUint8(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU1();
        }
        private int data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public int data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefInt8 extends KaitaiStruct {
        public static GucefInt8 fromFile(String fileName) throws IOException {
            return new GucefInt8(new ByteBufferKaitaiStream(fileName));
        }

        public GucefInt8(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefInt8(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefInt8(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readS1();
        }
        private byte data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public byte data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefInt16 extends KaitaiStruct {
        public static GucefInt16 fromFile(String fileName) throws IOException {
            return new GucefInt16(new ByteBufferKaitaiStream(fileName));
        }

        public GucefInt16(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefInt16(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefInt16(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readS2le();
        }
        private short data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public short data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefFloat64 extends KaitaiStruct {
        public static GucefFloat64 fromFile(String fileName) throws IOException {
            return new GucefFloat64(new ByteBufferKaitaiStream(fileName));
        }

        public GucefFloat64(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefFloat64(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefFloat64(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readF8le();
        }
        private double data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public double data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefLeTimestampInMsSinceUnixEpoch extends KaitaiStruct {
        public static GucefLeTimestampInMsSinceUnixEpoch fromFile(String fileName) throws IOException {
            return new GucefLeTimestampInMsSinceUnixEpoch(new ByteBufferKaitaiStream(fileName));
        }

        public GucefLeTimestampInMsSinceUnixEpoch(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefLeTimestampInMsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefLeTimestampInMsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU8le();
        }
        private long data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public long data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefInt32 extends KaitaiStruct {
        public static GucefInt32 fromFile(String fileName) throws IOException {
            return new GucefInt32(new ByteBufferKaitaiStream(fileName));
        }

        public GucefInt32(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefInt32(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefInt32(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readS4le();
        }
        private int data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public int data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefBeTimestampInSecsSinceUnixEpoch extends KaitaiStruct {
        public static GucefBeTimestampInSecsSinceUnixEpoch fromFile(String fileName) throws IOException {
            return new GucefBeTimestampInSecsSinceUnixEpoch(new ByteBufferKaitaiStream(fileName));
        }

        public GucefBeTimestampInSecsSinceUnixEpoch(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefBeTimestampInSecsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefBeTimestampInSecsSinceUnixEpoch(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU8le();
        }
        private long data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public long data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefFloat32 extends KaitaiStruct {
        public static GucefFloat32 fromFile(String fileName) throws IOException {
            return new GucefFloat32(new ByteBufferKaitaiStream(fileName));
        }

        public GucefFloat32(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefFloat32(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefFloat32(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readF4le();
        }
        private float data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public float data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefUint32 extends KaitaiStruct {
        public static GucefUint32 fromFile(String fileName) throws IOException {
            return new GucefUint32(new ByteBufferKaitaiStream(fileName));
        }

        public GucefUint32(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefUint32(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefUint32(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readU4le();
        }
        private long data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public long data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    public static class GucefBooleanInt32 extends KaitaiStruct {
        public static GucefBooleanInt32 fromFile(String fileName) throws IOException {
            return new GucefBooleanInt32(new ByteBufferKaitaiStream(fileName));
        }

        public GucefBooleanInt32(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefBooleanInt32(KaitaiStream _io, GucefCoreVariant _parent) {
            this(_io, _parent, null);
        }

        public GucefBooleanInt32(KaitaiStream _io, GucefCoreVariant _parent, GucefCoreVariant _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.data = this._io.readS4le();
        }
        private int data;
        private GucefCoreVariant _root;
        private GucefCoreVariant _parent;
        public int data() { return data; }
        public GucefCoreVariant _root() { return _root; }
        public GucefCoreVariant _parent() { return _parent; }
    }
    private GucefDatatypes typeId;
    private KaitaiStruct payload;
    private GucefCoreVariant _root;
    private KaitaiStruct _parent;
    public GucefDatatypes typeId() { return typeId; }
    public KaitaiStruct payload() { return payload; }
    public GucefCoreVariant _root() { return _root; }
    public KaitaiStruct _parent() { return _parent; }
}
