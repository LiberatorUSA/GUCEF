// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

(function (root, factory) {
  if (typeof define === 'function' && define.amd) {
    define(['kaitai-struct/KaitaiStream', './GucefPubsubMsgcontainerFooter', './GucefCoreVariant', './GucefPubsubMsgcontainerHeader'], factory);
  } else if (typeof module === 'object' && module.exports) {
    module.exports = factory(require('kaitai-struct/KaitaiStream'), require('./GucefPubsubMsgcontainerFooter'), require('./GucefCoreVariant'), require('./GucefPubsubMsgcontainerHeader'));
  } else {
    root.GucefPubsubMsgcontainer = factory(root.KaitaiStream, root.GucefPubsubMsgcontainerFooter, root.GucefCoreVariant, root.GucefPubsubMsgcontainerHeader);
  }
}(typeof self !== 'undefined' ? self : this, function (KaitaiStream, GucefPubsubMsgcontainerFooter, GucefCoreVariant, GucefPubsubMsgcontainerHeader) {
var GucefPubsubMsgcontainer = (function() {
  function GucefPubsubMsgcontainer(_io, _parent, _root) {
    this._io = _io;
    this._parent = _parent;
    this._root = _root || this;

    this._read();
  }
  GucefPubsubMsgcontainer.prototype._read = function() {
    this.header = new GucefPubsubMsgcontainerHeader(this._io, this, null);
    this.pubsubmsg = [];
    var i = 0;
    do {
      var _ = new GucefPubsubMsg(this._io, this, this._root);
      this.pubsubmsg.push(_);
      i++;
    } while (!(this._io.pos == (((this._io.size - (this.msgOffsetIdxSize * 4)) - 4) - 10)));
    this.footer = new GucefPubsubMsgcontainerFooter(this._io, this, null);
  }

  var GucefPubsubMsg = GucefPubsubMsgcontainer.GucefPubsubMsg = (function() {
    function GucefPubsubMsg(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefPubsubMsg.prototype._read = function() {
      if ((this._root.header.options & (1 << 1)) != 0) {
        switch ((this._root.header.options & (1 << 2))) {
        case 0:
          this.msgDateTime = this._io.readU4le();
          break;
        default:
          this.msgDateTime = this._io.readU8le();
          break;
        }
      }
      if ((this._root.header.options & (1 << 3)) != 0) {
        this.msgid = new GucefCoreVariant(this._io, this, null);
      }
      if ((this._root.header.options & (1 << 4)) != 0) {
        this.msgIndex = new GucefCoreVariant(this._io, this, null);
      }
      if ((this._root.header.options & (1 << 5)) != 0) {
        this.primaryPayloadIncluded = new GucefCoreVariant(this._io, this, null);
      }
      if ((this._root.header.options & (1 << 6)) != 0) {
        this.nrOfKvPairs = this._io.readU4le();
      }
      if ((this._root.header.options & (1 << 6)) != 0) {
        this.keyValuePairs = [];
        for (var i = 0; i < this.nrOfKvPairs; i++) {
          this.keyValuePairs.push(new KvPair(this._io, this, this._root));
        }
      }
      if ((this._root.header.options & (1 << 7)) != 0) {
        this.nrOfMetadataKvPairs = this._io.readU4le();
      }
      if ((this._root.header.options & (1 << 7)) != 0) {
        this.metadataKeyValuePairs = [];
        for (var i = 0; i < this.nrOfMetadataKvPairs; i++) {
          this.metadataKeyValuePairs.push(new KvPair(this._io, this, this._root));
        }
      }
    }

    return GucefPubsubMsg;
  })();

  var KvPair = GucefPubsubMsgcontainer.KvPair = (function() {
    function KvPair(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    KvPair.prototype._read = function() {
      this.key = new GucefCoreVariant(this._io, this, null);
      this.value = new GucefCoreVariant(this._io, this, null);
    }

    return KvPair;
  })();
  Object.defineProperty(GucefPubsubMsgcontainer.prototype, 'msgOffsetIdxSize', {
    get: function() {
      if (this._m_msgOffsetIdxSize !== undefined)
        return this._m_msgOffsetIdxSize;
      var _pos = this._io.pos;
      this._io.seek((this._io.size - 14));
      this._m_msgOffsetIdxSize = this._io.readU4le();
      this._io.seek(_pos);
      return this._m_msgOffsetIdxSize;
    }
  });

  return GucefPubsubMsgcontainer;
})();
return GucefPubsubMsgcontainer;
}));
