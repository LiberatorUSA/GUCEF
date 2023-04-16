import kaitai_struct_nim_runtime
import options

type
  GucefPubsubMsgcontainerFooter* = ref object of KaitaiStruct
    `msgOffsetIdx`*: seq[uint32]
    `msgOffsetIdxSize`*: uint32
    `magicText`*: seq[byte]
    `parent`*: KaitaiStruct
    `moisInst`: uint32
    `moisInstFlag`: bool

proc read*(_: typedesc[GucefPubsubMsgcontainerFooter], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefPubsubMsgcontainerFooter

proc mois*(this: GucefPubsubMsgcontainerFooter): uint32

proc read*(_: typedesc[GucefPubsubMsgcontainerFooter], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefPubsubMsgcontainerFooter =
  template this: untyped = result
  this = new(GucefPubsubMsgcontainerFooter)
  let root = if root == nil: cast[GucefPubsubMsgcontainerFooter](this) else: cast[GucefPubsubMsgcontainerFooter](root)
  this.io = io
  this.root = root
  this.parent = parent

  for i in 0 ..< int(this.mois):
    let it = this.io.readU4le()
    this.msgOffsetIdx.add(it)
  let msgOffsetIdxSizeExpr = this.io.readU4le()
  this.msgOffsetIdxSize = msgOffsetIdxSizeExpr
  let magicTextExpr = this.io.readBytes(int(10))
  this.magicText = magicTextExpr

proc mois(this: GucefPubsubMsgcontainerFooter): uint32 = 
  if this.moisInstFlag:
    return this.moisInst
  let pos = this.io.pos()
  this.io.seek(int((this.io.size - 14)))
  let moisInstExpr = this.io.readU4le()
  this.moisInst = moisInstExpr
  this.io.seek(pos)
  this.moisInstFlag = true
  return this.moisInst

proc fromFile*(_: typedesc[GucefPubsubMsgcontainerFooter], filename: string): GucefPubsubMsgcontainerFooter =
  GucefPubsubMsgcontainerFooter.read(newKaitaiFileStream(filename), nil, nil)

