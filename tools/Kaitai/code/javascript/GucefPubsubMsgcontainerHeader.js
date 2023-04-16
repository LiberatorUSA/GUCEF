// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

(function (root, factory) {
  if (typeof define === 'function' && define.amd) {
    define(['kaitai-struct/KaitaiStream'], factory);
  } else if (typeof module === 'object' && module.exports) {
    module.exports = factory(require('kaitai-struct/KaitaiStream'));
  } else {
    root.GucefPubsubMsgcontainerHeader = factory(root.KaitaiStream);
  }
}(typeof self !== 'undefined' ? self : this, function (KaitaiStream) {
var GucefPubsubMsgcontainerHeader = (function() {
  function GucefPubsubMsgcontainerHeader(_io, _parent, _root) {
    this._io = _io;
    this._parent = _parent;
    this._root = _root || this;

    this._read();
  }
  GucefPubsubMsgcontainerHeader.prototype._read = function() {
    this.magicText = this._io.readBytes(10);
    if (!((KaitaiStream.byteArrayCompare(this.magicText, [80, 85, 66, 83, 85, 66, 77, 83, 71, 83]) == 0))) {
      throw new KaitaiStream.ValidationNotEqualError([80, 85, 66, 83, 85, 66, 77, 83, 71, 83], this.magicText, this._io, "/seq/0");
    }
    this.version = this._io.readU1();
    this.options = this._io.readU4le();
  }

  return GucefPubsubMsgcontainerHeader;
})();
return GucefPubsubMsgcontainerHeader;
}));
