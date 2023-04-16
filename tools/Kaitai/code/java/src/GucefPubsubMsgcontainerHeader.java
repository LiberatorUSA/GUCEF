// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import io.kaitai.struct.ByteBufferKaitaiStream;
import io.kaitai.struct.KaitaiStruct;
import io.kaitai.struct.KaitaiStream;
import java.io.IOException;
import java.util.Arrays;

public class GucefPubsubMsgcontainerHeader extends KaitaiStruct {
    public static GucefPubsubMsgcontainerHeader fromFile(String fileName) throws IOException {
        return new GucefPubsubMsgcontainerHeader(new ByteBufferKaitaiStream(fileName));
    }

    public GucefPubsubMsgcontainerHeader(KaitaiStream _io) {
        this(_io, null, null);
    }

    public GucefPubsubMsgcontainerHeader(KaitaiStream _io, KaitaiStruct _parent) {
        this(_io, _parent, null);
    }

    public GucefPubsubMsgcontainerHeader(KaitaiStream _io, KaitaiStruct _parent, GucefPubsubMsgcontainerHeader _root) {
        super(_io);
        this._parent = _parent;
        this._root = _root == null ? this : _root;
        _read();
    }
    private void _read() {
        this.magicText = this._io.readBytes(10);
        if (!(Arrays.equals(magicText(), new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }))) {
            throw new KaitaiStream.ValidationNotEqualError(new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }, magicText(), _io(), "/seq/0");
        }
        this.version = this._io.readU1();
        this.options = this._io.readU4le();
    }
    private byte[] magicText;
    private int version;
    private long options;
    private GucefPubsubMsgcontainerHeader _root;
    private KaitaiStruct _parent;
    public byte[] magicText() { return magicText; }
    public int version() { return version; }
    public long options() { return options; }
    public GucefPubsubMsgcontainerHeader _root() { return _root; }
    public KaitaiStruct _parent() { return _parent; }
}
