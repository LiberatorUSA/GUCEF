import kaitai_struct_nim_runtime
import options
import gucef_core_dynamic

import "gucef_core_dynamic"
type
  GucefCoreVariant* = ref object of KaitaiStruct
    `typeId`*: GucefCoreVariant_GucefDatatypes
    `payload`*: KaitaiStruct
    `parent`*: KaitaiStruct
  GucefCoreVariant_GucefDatatypes* = enum
    uint8 = 1
    int8 = 2
    uint16 = 3
    int16 = 5
    uint32 = 7
    int32 = 9
    uint64 = 11
    int64 = 13
    float32 = 15
    float64 = 17
    ascii_string = 21
    utf8_string = 22
    utf16_le_string = 23
    utf16_be_string = 24
    utf32_string = 25
    boolean_int32 = 30
    boolean_ascii_string = 31
    boolean_utf8_string = 32
    binary_blob = 40
    binary_bsob = 41
    datetime_iso8601_ascii_string = 50
    datetime_iso8601_utf8_string = 51
    le_timestamp_in_secs_since_unix_epoch = 55
    be_timestamp_in_secs_since_unix_epoch = 56
    le_timestamp_in_ms_since_unix_epoch = 57
    be_timestamp_in_ms_since_unix_epoch = 58
  GucefCoreVariant_GucefInt64* = ref object of KaitaiStruct
    `data`*: int64
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefBsob* = ref object of KaitaiStruct
    `data`*: uint8
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch* = ref object of KaitaiStruct
    `data`*: uint64
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch* = ref object of KaitaiStruct
    `data`*: uint64
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefUint16* = ref object of KaitaiStruct
    `data`*: uint16
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefUint64* = ref object of KaitaiStruct
    `data`*: uint64
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefUint8* = ref object of KaitaiStruct
    `data`*: uint8
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefInt8* = ref object of KaitaiStruct
    `data`*: int8
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefInt16* = ref object of KaitaiStruct
    `data`*: int16
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefFloat64* = ref object of KaitaiStruct
    `data`*: float64
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch* = ref object of KaitaiStruct
    `data`*: uint64
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefInt32* = ref object of KaitaiStruct
    `data`*: int32
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch* = ref object of KaitaiStruct
    `data`*: uint64
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefFloat32* = ref object of KaitaiStruct
    `data`*: float32
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefUint32* = ref object of KaitaiStruct
    `data`*: uint32
    `parent`*: GucefCoreVariant
  GucefCoreVariant_GucefBooleanInt32* = ref object of KaitaiStruct
    `data`*: int32
    `parent`*: GucefCoreVariant

proc read*(_: typedesc[GucefCoreVariant], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefCoreVariant
proc read*(_: typedesc[GucefCoreVariant_GucefInt64], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt64
proc read*(_: typedesc[GucefCoreVariant_GucefBsob], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBsob
proc read*(_: typedesc[GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch
proc read*(_: typedesc[GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch
proc read*(_: typedesc[GucefCoreVariant_GucefUint16], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint16
proc read*(_: typedesc[GucefCoreVariant_GucefUint64], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint64
proc read*(_: typedesc[GucefCoreVariant_GucefUint8], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint8
proc read*(_: typedesc[GucefCoreVariant_GucefInt8], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt8
proc read*(_: typedesc[GucefCoreVariant_GucefInt16], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt16
proc read*(_: typedesc[GucefCoreVariant_GucefFloat64], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefFloat64
proc read*(_: typedesc[GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch
proc read*(_: typedesc[GucefCoreVariant_GucefInt32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt32
proc read*(_: typedesc[GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch
proc read*(_: typedesc[GucefCoreVariant_GucefFloat32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefFloat32
proc read*(_: typedesc[GucefCoreVariant_GucefUint32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint32
proc read*(_: typedesc[GucefCoreVariant_GucefBooleanInt32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBooleanInt32


proc read*(_: typedesc[GucefCoreVariant], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefCoreVariant =
  template this: untyped = result
  this = new(GucefCoreVariant)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let typeIdExpr = GucefCoreVariant_GucefDatatypes(this.io.readU1())
  this.typeId = typeIdExpr
  block:
    let on = this.typeId
    if on == gucef_core_variant.datetime_iso8601_ascii_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.utf16_le_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.int16:
      let payloadExpr = GucefCoreVariant_GucefInt16.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.float32:
      let payloadExpr = GucefCoreVariant_GucefFloat32.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.ascii_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.uint16:
      let payloadExpr = GucefCoreVariant_GucefUint16.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.uint64:
      let payloadExpr = GucefCoreVariant_GucefUint64.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.int32:
      let payloadExpr = GucefCoreVariant_GucefInt32.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.uint8:
      let payloadExpr = GucefCoreVariant_GucefUint8.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.uint32:
      let payloadExpr = GucefCoreVariant_GucefUint32.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.float64:
      let payloadExpr = GucefCoreVariant_GucefFloat64.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.be_timestamp_in_secs_since_unix_epoch:
      let payloadExpr = GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.be_timestamp_in_ms_since_unix_epoch:
      let payloadExpr = GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.utf32_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.boolean_utf8_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.binary_blob:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.le_timestamp_in_ms_since_unix_epoch:
      let payloadExpr = GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.binary_bsob:
      let payloadExpr = GucefCoreVariant_GucefBsob.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.boolean_ascii_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.utf8_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.int8:
      let payloadExpr = GucefCoreVariant_GucefInt8.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.boolean_int32:
      let payloadExpr = GucefCoreVariant_GucefBooleanInt32.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.le_timestamp_in_secs_since_unix_epoch:
      let payloadExpr = GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.datetime_iso8601_utf8_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.int64:
      let payloadExpr = GucefCoreVariant_GucefInt64.read(this.io, this.root, this)
      this.payload = payloadExpr
    elif on == gucef_core_variant.utf16_be_string:
      let payloadExpr = GucefCoreDynamic.read(this.io, this.root, this)
      this.payload = payloadExpr

proc fromFile*(_: typedesc[GucefCoreVariant], filename: string): GucefCoreVariant =
  GucefCoreVariant.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefInt64], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt64 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefInt64)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readS8le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefInt64], filename: string): GucefCoreVariant_GucefInt64 =
  GucefCoreVariant_GucefInt64.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefBsob], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBsob =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefBsob)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU1()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefBsob], filename: string): GucefCoreVariant_GucefBsob =
  GucefCoreVariant_GucefBsob.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU8le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch], filename: string): GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch =
  GucefCoreVariant_GucefBeTimestampInMsSinceUnixEpoch.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU8le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch], filename: string): GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch =
  GucefCoreVariant_GucefLeTimestampInSecsSinceUnixEpoch.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefUint16], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint16 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefUint16)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU2le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefUint16], filename: string): GucefCoreVariant_GucefUint16 =
  GucefCoreVariant_GucefUint16.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefUint64], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint64 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefUint64)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU8le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefUint64], filename: string): GucefCoreVariant_GucefUint64 =
  GucefCoreVariant_GucefUint64.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefUint8], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint8 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefUint8)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU1()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefUint8], filename: string): GucefCoreVariant_GucefUint8 =
  GucefCoreVariant_GucefUint8.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefInt8], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt8 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefInt8)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readS1()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefInt8], filename: string): GucefCoreVariant_GucefInt8 =
  GucefCoreVariant_GucefInt8.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefInt16], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt16 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefInt16)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readS2le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefInt16], filename: string): GucefCoreVariant_GucefInt16 =
  GucefCoreVariant_GucefInt16.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefFloat64], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefFloat64 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefFloat64)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readF8le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefFloat64], filename: string): GucefCoreVariant_GucefFloat64 =
  GucefCoreVariant_GucefFloat64.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU8le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch], filename: string): GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch =
  GucefCoreVariant_GucefLeTimestampInMsSinceUnixEpoch.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefInt32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefInt32 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefInt32)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readS4le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefInt32], filename: string): GucefCoreVariant_GucefInt32 =
  GucefCoreVariant_GucefInt32.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU8le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch], filename: string): GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch =
  GucefCoreVariant_GucefBeTimestampInSecsSinceUnixEpoch.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefFloat32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefFloat32 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefFloat32)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readF4le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefFloat32], filename: string): GucefCoreVariant_GucefFloat32 =
  GucefCoreVariant_GucefFloat32.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefUint32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefUint32 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefUint32)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readU4le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefUint32], filename: string): GucefCoreVariant_GucefUint32 =
  GucefCoreVariant_GucefUint32.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefCoreVariant_GucefBooleanInt32], io: KaitaiStream, root: KaitaiStruct, parent: GucefCoreVariant): GucefCoreVariant_GucefBooleanInt32 =
  template this: untyped = result
  this = new(GucefCoreVariant_GucefBooleanInt32)
  let root = if root == nil: cast[GucefCoreVariant](this) else: cast[GucefCoreVariant](root)
  this.io = io
  this.root = root
  this.parent = parent

  let dataExpr = this.io.readS4le()
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreVariant_GucefBooleanInt32], filename: string): GucefCoreVariant_GucefBooleanInt32 =
  GucefCoreVariant_GucefBooleanInt32.read(newKaitaiFileStream(filename), nil, nil)

