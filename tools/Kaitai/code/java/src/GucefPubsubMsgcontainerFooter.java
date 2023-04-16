// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import io.kaitai.struct.ByteBufferKaitaiStream;
import io.kaitai.struct.KaitaiStruct;
import io.kaitai.struct.KaitaiStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

public class GucefPubsubMsgcontainerFooter extends KaitaiStruct {
    public static GucefPubsubMsgcontainerFooter fromFile(String fileName) throws IOException {
        return new GucefPubsubMsgcontainerFooter(new ByteBufferKaitaiStream(fileName));
    }

    public GucefPubsubMsgcontainerFooter(KaitaiStream _io) {
        this(_io, null, null);
    }

    public GucefPubsubMsgcontainerFooter(KaitaiStream _io, KaitaiStruct _parent) {
        this(_io, _parent, null);
    }

    public GucefPubsubMsgcontainerFooter(KaitaiStream _io, KaitaiStruct _parent, GucefPubsubMsgcontainerFooter _root) {
        super(_io);
        this._parent = _parent;
        this._root = _root == null ? this : _root;
        _read();
    }
    private void _read() {
        this.msgOffsetIdx = new ArrayList<Long>();
        for (int i = 0; i < mois(); i++) {
            this.msgOffsetIdx.add(this._io.readU4le());
        }
        this.msgOffsetIdxSize = this._io.readU4le();
        this.magicText = this._io.readBytes(10);
        if (!(Arrays.equals(magicText(), new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }))) {
            throw new KaitaiStream.ValidationNotEqualError(new byte[] { 80, 85, 66, 83, 85, 66, 77, 83, 71, 83 }, magicText(), _io(), "/seq/2");
        }
    }
    private Long mois;
    public Long mois() {
        if (this.mois != null)
            return this.mois;
        long _pos = this._io.pos();
        this._io.seek((_io().size() - 14));
        this.mois = this._io.readU4le();
        this._io.seek(_pos);
        return this.mois;
    }
    private ArrayList<Long> msgOffsetIdx;
    private long msgOffsetIdxSize;
    private byte[] magicText;
    private GucefPubsubMsgcontainerFooter _root;
    private KaitaiStruct _parent;
    public ArrayList<Long> msgOffsetIdx() { return msgOffsetIdx; }
    public long msgOffsetIdxSize() { return msgOffsetIdxSize; }
    public byte[] magicText() { return magicText; }
    public GucefPubsubMsgcontainerFooter _root() { return _root; }
    public KaitaiStruct _parent() { return _parent; }
}
