// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

import io.kaitai.struct.ByteBufferKaitaiStream;
import io.kaitai.struct.KaitaiStruct;
import io.kaitai.struct.KaitaiStream;
import java.io.IOException;
import java.util.ArrayList;

public class GucefPubsubMsgcontainer extends KaitaiStruct {
    public static GucefPubsubMsgcontainer fromFile(String fileName) throws IOException {
        return new GucefPubsubMsgcontainer(new ByteBufferKaitaiStream(fileName));
    }

    public GucefPubsubMsgcontainer(KaitaiStream _io) {
        this(_io, null, null);
    }

    public GucefPubsubMsgcontainer(KaitaiStream _io, KaitaiStruct _parent) {
        this(_io, _parent, null);
    }

    public GucefPubsubMsgcontainer(KaitaiStream _io, KaitaiStruct _parent, GucefPubsubMsgcontainer _root) {
        super(_io);
        this._parent = _parent;
        this._root = _root == null ? this : _root;
        _read();
    }
    private void _read() {
        this.header = new GucefPubsubMsgcontainerHeader(this._io);
        this.pubsubmsg = new ArrayList<GucefPubsubMsg>();
        {
            GucefPubsubMsg _it;
            int i = 0;
            do {
                _it = new GucefPubsubMsg(this._io, this, _root);
                this.pubsubmsg.add(_it);
                i++;
            } while (!(_io().pos() == (((_io().size() - (msgOffsetIdxSize() * 4)) - 4) - 10)));
        }
        this.footer = new GucefPubsubMsgcontainerFooter(this._io);
    }
    public static class GucefPubsubMsg extends KaitaiStruct {
        public static GucefPubsubMsg fromFile(String fileName) throws IOException {
            return new GucefPubsubMsg(new ByteBufferKaitaiStream(fileName));
        }

        public GucefPubsubMsg(KaitaiStream _io) {
            this(_io, null, null);
        }

        public GucefPubsubMsg(KaitaiStream _io, GucefPubsubMsgcontainer _parent) {
            this(_io, _parent, null);
        }

        public GucefPubsubMsg(KaitaiStream _io, GucefPubsubMsgcontainer _parent, GucefPubsubMsgcontainer _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            if ((_root().header().options() & (1 << 1)) != 0) {
                switch ((_root().header().options() & (1 << 2))) {
                case 0: {
                    this.msgDateTime = (long) (this._io.readU4le());
                    break;
                }
                default: {
                    this.msgDateTime = this._io.readU8le();
                    break;
                }
                }
            }
            if ((_root().header().options() & (1 << 3)) != 0) {
                this.msgid = new GucefCoreVariant(this._io);
            }
            if ((_root().header().options() & (1 << 4)) != 0) {
                this.msgIndex = new GucefCoreVariant(this._io);
            }
            if ((_root().header().options() & (1 << 5)) != 0) {
                this.primaryPayloadIncluded = new GucefCoreVariant(this._io);
            }
            if ((_root().header().options() & (1 << 6)) != 0) {
                this.nrOfKvPairs = this._io.readU4le();
            }
            if ((_root().header().options() & (1 << 6)) != 0) {
                this.keyValuePairs = new ArrayList<KvPair>();
                for (int i = 0; i < nrOfKvPairs(); i++) {
                    this.keyValuePairs.add(new KvPair(this._io, this, _root));
                }
            }
            if ((_root().header().options() & (1 << 7)) != 0) {
                this.nrOfMetadataKvPairs = this._io.readU4le();
            }
            if ((_root().header().options() & (1 << 7)) != 0) {
                this.metadataKeyValuePairs = new ArrayList<KvPair>();
                for (int i = 0; i < nrOfMetadataKvPairs(); i++) {
                    this.metadataKeyValuePairs.add(new KvPair(this._io, this, _root));
                }
            }
        }
        private Long msgDateTime;
        private GucefCoreVariant msgid;
        private GucefCoreVariant msgIndex;
        private GucefCoreVariant primaryPayloadIncluded;
        private Long nrOfKvPairs;
        private ArrayList<KvPair> keyValuePairs;
        private Long nrOfMetadataKvPairs;
        private ArrayList<KvPair> metadataKeyValuePairs;
        private GucefPubsubMsgcontainer _root;
        private GucefPubsubMsgcontainer _parent;
        public Long msgDateTime() { return msgDateTime; }
        public GucefCoreVariant msgid() { return msgid; }
        public GucefCoreVariant msgIndex() { return msgIndex; }
        public GucefCoreVariant primaryPayloadIncluded() { return primaryPayloadIncluded; }
        public Long nrOfKvPairs() { return nrOfKvPairs; }
        public ArrayList<KvPair> keyValuePairs() { return keyValuePairs; }
        public Long nrOfMetadataKvPairs() { return nrOfMetadataKvPairs; }
        public ArrayList<KvPair> metadataKeyValuePairs() { return metadataKeyValuePairs; }
        public GucefPubsubMsgcontainer _root() { return _root; }
        public GucefPubsubMsgcontainer _parent() { return _parent; }
    }
    public static class KvPair extends KaitaiStruct {
        public static KvPair fromFile(String fileName) throws IOException {
            return new KvPair(new ByteBufferKaitaiStream(fileName));
        }

        public KvPair(KaitaiStream _io) {
            this(_io, null, null);
        }

        public KvPair(KaitaiStream _io, GucefPubsubMsgcontainer.GucefPubsubMsg _parent) {
            this(_io, _parent, null);
        }

        public KvPair(KaitaiStream _io, GucefPubsubMsgcontainer.GucefPubsubMsg _parent, GucefPubsubMsgcontainer _root) {
            super(_io);
            this._parent = _parent;
            this._root = _root;
            _read();
        }
        private void _read() {
            this.key = new GucefCoreVariant(this._io);
            this.value = new GucefCoreVariant(this._io);
        }
        private GucefCoreVariant key;
        private GucefCoreVariant value;
        private GucefPubsubMsgcontainer _root;
        private GucefPubsubMsgcontainer.GucefPubsubMsg _parent;
        public GucefCoreVariant key() { return key; }
        public GucefCoreVariant value() { return value; }
        public GucefPubsubMsgcontainer _root() { return _root; }
        public GucefPubsubMsgcontainer.GucefPubsubMsg _parent() { return _parent; }
    }
    private Long msgOffsetIdxSize;
    public Long msgOffsetIdxSize() {
        if (this.msgOffsetIdxSize != null)
            return this.msgOffsetIdxSize;
        long _pos = this._io.pos();
        this._io.seek((_io().size() - 14));
        this.msgOffsetIdxSize = this._io.readU4le();
        this._io.seek(_pos);
        return this.msgOffsetIdxSize;
    }
    private GucefPubsubMsgcontainerHeader header;
    private ArrayList<GucefPubsubMsg> pubsubmsg;
    private GucefPubsubMsgcontainerFooter footer;
    private GucefPubsubMsgcontainer _root;
    private KaitaiStruct _parent;
    public GucefPubsubMsgcontainerHeader header() { return header; }
    public ArrayList<GucefPubsubMsg> pubsubmsg() { return pubsubmsg; }
    public GucefPubsubMsgcontainerFooter footer() { return footer; }
    public GucefPubsubMsgcontainer _root() { return _root; }
    public KaitaiStruct _parent() { return _parent; }
}
