// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

(function (root, factory) {
  if (typeof define === 'function' && define.amd) {
    define(['kaitai-struct/KaitaiStream'], factory);
  } else if (typeof module === 'object' && module.exports) {
    module.exports = factory(require('kaitai-struct/KaitaiStream'));
  } else {
    root.GucefCoreDynamic = factory(root.KaitaiStream);
  }
}(typeof self !== 'undefined' ? self : this, function (KaitaiStream) {
var GucefCoreDynamic = (function() {
  function GucefCoreDynamic(_io, _parent, _root) {
    this._io = _io;
    this._parent = _parent;
    this._root = _root || this;

    this._read();
  }
  GucefCoreDynamic.prototype._read = function() {
    this.payloadSize = this._io.readU4le();
    this.data = this._io.readBytes(this.payloadSize);
  }

  return GucefCoreDynamic;
})();
return GucefCoreDynamic;
}));
