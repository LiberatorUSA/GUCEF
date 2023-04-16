// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import io.kaitai.struct.ByteBufferKaitaiStream;
import io.kaitai.struct.KaitaiStruct;
import io.kaitai.struct.KaitaiStream;
import java.io.IOException;

public class GucefCoreDynamic extends KaitaiStruct {
    public static GucefCoreDynamic fromFile(String fileName) throws IOException {
        return new GucefCoreDynamic(new ByteBufferKaitaiStream(fileName));
    }

    public GucefCoreDynamic(KaitaiStream _io) {
        this(_io, null, null);
    }

    public GucefCoreDynamic(KaitaiStream _io, KaitaiStruct _parent) {
        this(_io, _parent, null);
    }

    public GucefCoreDynamic(KaitaiStream _io, KaitaiStruct _parent, GucefCoreDynamic _root) {
        super(_io);
        this._parent = _parent;
        this._root = _root == null ? this : _root;
        _read();
    }
    private void _read() {
        this.payloadSize = this._io.readU4le();
        this.data = this._io.readBytes(payloadSize());
    }
    private long payloadSize;
    private byte[] data;
    private GucefCoreDynamic _root;
    private KaitaiStruct _parent;
    public long payloadSize() { return payloadSize; }
    public byte[] data() { return data; }
    public GucefCoreDynamic _root() { return _root; }
    public KaitaiStruct _parent() { return _parent; }
}
