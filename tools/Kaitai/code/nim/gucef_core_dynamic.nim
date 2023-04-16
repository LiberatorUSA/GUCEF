import kaitai_struct_nim_runtime
import options

type
  GucefCoreDynamic* = ref object of KaitaiStruct
    `payloadSize`*: uint32
    `data`*: seq[byte]
    `parent`*: KaitaiStruct

proc read*(_: typedesc[GucefCoreDynamic], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefCoreDynamic


proc read*(_: typedesc[GucefCoreDynamic], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefCoreDynamic =
  template this: untyped = result
  this = new(GucefCoreDynamic)
  let root = if root == nil: cast[GucefCoreDynamic](this) else: cast[GucefCoreDynamic](root)
  this.io = io
  this.root = root
  this.parent = parent

  let payloadSizeExpr = this.io.readU4le()
  this.payloadSize = payloadSizeExpr
  let dataExpr = this.io.readBytes(int(this.payloadSize))
  this.data = dataExpr

proc fromFile*(_: typedesc[GucefCoreDynamic], filename: string): GucefCoreDynamic =
  GucefCoreDynamic.read(newKaitaiFileStream(filename), nil, nil)

