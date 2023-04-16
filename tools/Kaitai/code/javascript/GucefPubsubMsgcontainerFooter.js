// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

(function (root, factory) {
  if (typeof define === 'function' && define.amd) {
    define(['kaitai-struct/KaitaiStream'], factory);
  } else if (typeof module === 'object' && module.exports) {
    module.exports = factory(require('kaitai-struct/KaitaiStream'));
  } else {
    root.GucefPubsubMsgcontainerFooter = factory(root.KaitaiStream);
  }
}(typeof self !== 'undefined' ? self : this, function (KaitaiStream) {
var GucefPubsubMsgcontainerFooter = (function() {
  function GucefPubsubMsgcontainerFooter(_io, _parent, _root) {
    this._io = _io;
    this._parent = _parent;
    this._root = _root || this;

    this._read();
  }
  GucefPubsubMsgcontainerFooter.prototype._read = function() {
    this.msgOffsetIdx = [];
    for (var i = 0; i < this.mois; i++) {
      this.msgOffsetIdx.push(this._io.readU4le());
    }
    this.msgOffsetIdxSize = this._io.readU4le();
    this.magicText = this._io.readBytes(10);
    if (!((KaitaiStream.byteArrayCompare(this.magicText, [80, 85, 66, 83, 85, 66, 77, 83, 71, 83]) == 0))) {
      throw new KaitaiStream.ValidationNotEqualError([80, 85, 66, 83, 85, 66, 77, 83, 71, 83], this.magicText, this._io, "/seq/2");
    }
  }
  Object.defineProperty(GucefPubsubMsgcontainerFooter.prototype, 'mois', {
    get: function() {
      if (this._m_mois !== undefined)
        return this._m_mois;
      var _pos = this._io.pos;
      this._io.seek((this._io.size - 14));
      this._m_mois = this._io.readU4le();
      this._io.seek(_pos);
      return this._m_mois;
    }
  });

  return GucefPubsubMsgcontainerFooter;
})();
return GucefPubsubMsgcontainerFooter;
}));
