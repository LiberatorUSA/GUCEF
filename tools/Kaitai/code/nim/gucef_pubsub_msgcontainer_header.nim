import kaitai_struct_nim_runtime
import options

type
  GucefPubsubMsgcontainerHeader* = ref object of KaitaiStruct
    `magicText`*: seq[byte]
    `version`*: uint8
    `options`*: uint32
    `parent`*: KaitaiStruct

proc read*(_: typedesc[GucefPubsubMsgcontainerHeader], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefPubsubMsgcontainerHeader


proc read*(_: typedesc[GucefPubsubMsgcontainerHeader], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefPubsubMsgcontainerHeader =
  template this: untyped = result
  this = new(GucefPubsubMsgcontainerHeader)
  let root = if root == nil: cast[GucefPubsubMsgcontainerHeader](this) else: cast[GucefPubsubMsgcontainerHeader](root)
  this.io = io
  this.root = root
  this.parent = parent

  let magicTextExpr = this.io.readBytes(int(10))
  this.magicText = magicTextExpr
  let versionExpr = this.io.readU1()
  this.version = versionExpr
  let optionsExpr = this.io.readU4le()
  this.options = optionsExpr

proc fromFile*(_: typedesc[GucefPubsubMsgcontainerHeader], filename: string): GucefPubsubMsgcontainerHeader =
  GucefPubsubMsgcontainerHeader.read(newKaitaiFileStream(filename), nil, nil)

