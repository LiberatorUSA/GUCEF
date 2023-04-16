// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

(function (root, factory) {
  if (typeof define === 'function' && define.amd) {
    define(['kaitai-struct/KaitaiStream', './GucefCoreDynamic'], factory);
  } else if (typeof module === 'object' && module.exports) {
    module.exports = factory(require('kaitai-struct/KaitaiStream'), require('./GucefCoreDynamic'));
  } else {
    root.GucefCoreVariant = factory(root.KaitaiStream, root.GucefCoreDynamic);
  }
}(typeof self !== 'undefined' ? self : this, function (KaitaiStream, GucefCoreDynamic) {
var GucefCoreVariant = (function() {
  GucefCoreVariant.GucefDatatypes = Object.freeze({
    UINT8: 1,
    INT8: 2,
    UINT16: 3,
    INT16: 5,
    UINT32: 7,
    INT32: 9,
    UINT64: 11,
    INT64: 13,
    FLOAT32: 15,
    FLOAT64: 17,
    ASCII_STRING: 21,
    UTF8_STRING: 22,
    UTF16_LE_STRING: 23,
    UTF16_BE_STRING: 24,
    UTF32_STRING: 25,
    BOOLEAN_INT32: 30,
    BOOLEAN_ASCII_STRING: 31,
    BOOLEAN_UTF8_STRING: 32,
    BINARY_BLOB: 40,
    BINARY_BSOB: 41,
    DATETIME_ISO8601_ASCII_STRING: 50,
    DATETIME_ISO8601_UTF8_STRING: 51,
    LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH: 55,
    BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH: 56,
    LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH: 57,
    BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH: 58,

    1: "UINT8",
    2: "INT8",
    3: "UINT16",
    5: "INT16",
    7: "UINT32",
    9: "INT32",
    11: "UINT64",
    13: "INT64",
    15: "FLOAT32",
    17: "FLOAT64",
    21: "ASCII_STRING",
    22: "UTF8_STRING",
    23: "UTF16_LE_STRING",
    24: "UTF16_BE_STRING",
    25: "UTF32_STRING",
    30: "BOOLEAN_INT32",
    31: "BOOLEAN_ASCII_STRING",
    32: "BOOLEAN_UTF8_STRING",
    40: "BINARY_BLOB",
    41: "BINARY_BSOB",
    50: "DATETIME_ISO8601_ASCII_STRING",
    51: "DATETIME_ISO8601_UTF8_STRING",
    55: "LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH",
    56: "BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH",
    57: "LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH",
    58: "BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH",
  });

  function GucefCoreVariant(_io, _parent, _root) {
    this._io = _io;
    this._parent = _parent;
    this._root = _root || this;

    this._read();
  }
  GucefCoreVariant.prototype._read = function() {
    this.typeId = this._io.readU1();
    switch (this.typeId) {
    case GucefCoreVariant.GucefDatatypes.DATETIME_ISO8601_ASCII_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.UTF16_LE_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.INT16:
      this.payload = new GucefInt16(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.FLOAT32:
      this.payload = new GucefFloat32(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.ASCII_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.UINT16:
      this.payload = new GucefUint16(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.UINT64:
      this.payload = new GucefUint64(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.INT32:
      this.payload = new GucefInt32(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.UINT8:
      this.payload = new GucefUint8(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.UINT32:
      this.payload = new GucefUint32(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.FLOAT64:
      this.payload = new GucefFloat64(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH:
      this.payload = new GucefBeTimestampInSecsSinceUnixEpoch(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH:
      this.payload = new GucefBeTimestampInMsSinceUnixEpoch(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.UTF32_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.BOOLEAN_UTF8_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.BINARY_BLOB:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH:
      this.payload = new GucefLeTimestampInMsSinceUnixEpoch(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.BINARY_BSOB:
      this.payload = new GucefBsob(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.BOOLEAN_ASCII_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.UTF8_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.INT8:
      this.payload = new GucefInt8(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.BOOLEAN_INT32:
      this.payload = new GucefBooleanInt32(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH:
      this.payload = new GucefLeTimestampInSecsSinceUnixEpoch(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.DATETIME_ISO8601_UTF8_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    case GucefCoreVariant.GucefDatatypes.INT64:
      this.payload = new GucefInt64(this._io, this, this._root);
      break;
    case GucefCoreVariant.GucefDatatypes.UTF16_BE_STRING:
      this.payload = new GucefCoreDynamic(this._io, this, null);
      break;
    }
  }

  var GucefInt64 = GucefCoreVariant.GucefInt64 = (function() {
    function GucefInt64(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefInt64.prototype._read = function() {
      this.data = this._io.readS8le();
    }

    return GucefInt64;
  })();

  var GucefBsob = GucefCoreVariant.GucefBsob = (function() {
    function GucefBsob(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefBsob.prototype._read = function() {
      this.data = this._io.readU1();
    }

    return GucefBsob;
  })();

  var GucefBeTimestampInMsSinceUnixEpoch = GucefCoreVariant.GucefBeTimestampInMsSinceUnixEpoch = (function() {
    function GucefBeTimestampInMsSinceUnixEpoch(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefBeTimestampInMsSinceUnixEpoch.prototype._read = function() {
      this.data = this._io.readU8le();
    }

    return GucefBeTimestampInMsSinceUnixEpoch;
  })();

  var GucefLeTimestampInSecsSinceUnixEpoch = GucefCoreVariant.GucefLeTimestampInSecsSinceUnixEpoch = (function() {
    function GucefLeTimestampInSecsSinceUnixEpoch(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefLeTimestampInSecsSinceUnixEpoch.prototype._read = function() {
      this.data = this._io.readU8le();
    }

    return GucefLeTimestampInSecsSinceUnixEpoch;
  })();

  var GucefUint16 = GucefCoreVariant.GucefUint16 = (function() {
    function GucefUint16(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefUint16.prototype._read = function() {
      this.data = this._io.readU2le();
    }

    return GucefUint16;
  })();

  var GucefUint64 = GucefCoreVariant.GucefUint64 = (function() {
    function GucefUint64(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefUint64.prototype._read = function() {
      this.data = this._io.readU8le();
    }

    return GucefUint64;
  })();

  var GucefUint8 = GucefCoreVariant.GucefUint8 = (function() {
    function GucefUint8(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefUint8.prototype._read = function() {
      this.data = this._io.readU1();
    }

    return GucefUint8;
  })();

  var GucefInt8 = GucefCoreVariant.GucefInt8 = (function() {
    function GucefInt8(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefInt8.prototype._read = function() {
      this.data = this._io.readS1();
    }

    return GucefInt8;
  })();

  var GucefInt16 = GucefCoreVariant.GucefInt16 = (function() {
    function GucefInt16(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefInt16.prototype._read = function() {
      this.data = this._io.readS2le();
    }

    return GucefInt16;
  })();

  var GucefFloat64 = GucefCoreVariant.GucefFloat64 = (function() {
    function GucefFloat64(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefFloat64.prototype._read = function() {
      this.data = this._io.readF8le();
    }

    return GucefFloat64;
  })();

  var GucefLeTimestampInMsSinceUnixEpoch = GucefCoreVariant.GucefLeTimestampInMsSinceUnixEpoch = (function() {
    function GucefLeTimestampInMsSinceUnixEpoch(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefLeTimestampInMsSinceUnixEpoch.prototype._read = function() {
      this.data = this._io.readU8le();
    }

    return GucefLeTimestampInMsSinceUnixEpoch;
  })();

  var GucefInt32 = GucefCoreVariant.GucefInt32 = (function() {
    function GucefInt32(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefInt32.prototype._read = function() {
      this.data = this._io.readS4le();
    }

    return GucefInt32;
  })();

  var GucefBeTimestampInSecsSinceUnixEpoch = GucefCoreVariant.GucefBeTimestampInSecsSinceUnixEpoch = (function() {
    function GucefBeTimestampInSecsSinceUnixEpoch(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefBeTimestampInSecsSinceUnixEpoch.prototype._read = function() {
      this.data = this._io.readU8le();
    }

    return GucefBeTimestampInSecsSinceUnixEpoch;
  })();

  var GucefFloat32 = GucefCoreVariant.GucefFloat32 = (function() {
    function GucefFloat32(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefFloat32.prototype._read = function() {
      this.data = this._io.readF4le();
    }

    return GucefFloat32;
  })();

  var GucefUint32 = GucefCoreVariant.GucefUint32 = (function() {
    function GucefUint32(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefUint32.prototype._read = function() {
      this.data = this._io.readU4le();
    }

    return GucefUint32;
  })();

  var GucefBooleanInt32 = GucefCoreVariant.GucefBooleanInt32 = (function() {
    function GucefBooleanInt32(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;

      this._read();
    }
    GucefBooleanInt32.prototype._read = function() {
      this.data = this._io.readS4le();
    }

    return GucefBooleanInt32;
  })();

  return GucefCoreVariant;
})();
return GucefCoreVariant;
}));
