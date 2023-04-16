import kaitai_struct_nim_runtime
import options
import gucef_core_variant
import gucef_pubsub_msgcontainer_header
import gucef_pubsub_msgcontainer_footer

import "gucef_pubsub_msgcontainer_footer"
import "gucef_core_variant"
import "gucef_pubsub_msgcontainer_header"
type
  GucefPubsubMsgcontainer* = ref object of KaitaiStruct
    `header`*: GucefPubsubMsgcontainerHeader
    `pubsubmsg`*: seq[GucefPubsubMsgcontainer_GucefPubsubMsg]
    `footer`*: GucefPubsubMsgcontainerFooter
    `parent`*: KaitaiStruct
    `msgOffsetIdxSizeInst`: uint32
    `msgOffsetIdxSizeInstFlag`: bool
  GucefPubsubMsgcontainer_GucefPubsubMsg* = ref object of KaitaiStruct
    `msgDateTime`*: uint64
    `msgid`*: GucefCoreVariant
    `msgIndex`*: GucefCoreVariant
    `primaryPayloadIncluded`*: GucefCoreVariant
    `nrOfKvPairs`*: uint32
    `keyValuePairs`*: seq[GucefPubsubMsgcontainer_KvPair]
    `nrOfMetadataKvPairs`*: uint32
    `metadataKeyValuePairs`*: seq[GucefPubsubMsgcontainer_KvPair]
    `parent`*: GucefPubsubMsgcontainer
  GucefPubsubMsgcontainer_KvPair* = ref object of KaitaiStruct
    `key`*: GucefCoreVariant
    `value`*: GucefCoreVariant
    `parent`*: GucefPubsubMsgcontainer_GucefPubsubMsg

proc read*(_: typedesc[GucefPubsubMsgcontainer], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefPubsubMsgcontainer
proc read*(_: typedesc[GucefPubsubMsgcontainer_GucefPubsubMsg], io: KaitaiStream, root: KaitaiStruct, parent: GucefPubsubMsgcontainer): GucefPubsubMsgcontainer_GucefPubsubMsg
proc read*(_: typedesc[GucefPubsubMsgcontainer_KvPair], io: KaitaiStream, root: KaitaiStruct, parent: GucefPubsubMsgcontainer_GucefPubsubMsg): GucefPubsubMsgcontainer_KvPair

proc msgOffsetIdxSize*(this: GucefPubsubMsgcontainer): uint32

proc read*(_: typedesc[GucefPubsubMsgcontainer], io: KaitaiStream, root: KaitaiStruct, parent: KaitaiStruct): GucefPubsubMsgcontainer =
  template this: untyped = result
  this = new(GucefPubsubMsgcontainer)
  let root = if root == nil: cast[GucefPubsubMsgcontainer](this) else: cast[GucefPubsubMsgcontainer](root)
  this.io = io
  this.root = root
  this.parent = parent

  let headerExpr = GucefPubsubMsgcontainerHeader.read(this.io, this.root, this)
  this.header = headerExpr
  block:
    var i: int
    while true:
      let it = GucefPubsubMsgcontainer_GucefPubsubMsg.read(this.io, this.root, this)
      this.pubsubmsg.add(it)
      if this.io.pos == (((this.io.size - (this.msgOffsetIdxSize * 4)) - 4) - 10):
        break
      inc i
  let footerExpr = GucefPubsubMsgcontainerFooter.read(this.io, this.root, this)
  this.footer = footerExpr

proc msgOffsetIdxSize(this: GucefPubsubMsgcontainer): uint32 = 
  if this.msgOffsetIdxSizeInstFlag:
    return this.msgOffsetIdxSizeInst
  let pos = this.io.pos()
  this.io.seek(int((this.io.size - 14)))
  let msgOffsetIdxSizeInstExpr = this.io.readU4le()
  this.msgOffsetIdxSizeInst = msgOffsetIdxSizeInstExpr
  this.io.seek(pos)
  this.msgOffsetIdxSizeInstFlag = true
  return this.msgOffsetIdxSizeInst

proc fromFile*(_: typedesc[GucefPubsubMsgcontainer], filename: string): GucefPubsubMsgcontainer =
  GucefPubsubMsgcontainer.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefPubsubMsgcontainer_GucefPubsubMsg], io: KaitaiStream, root: KaitaiStruct, parent: GucefPubsubMsgcontainer): GucefPubsubMsgcontainer_GucefPubsubMsg =
  template this: untyped = result
  this = new(GucefPubsubMsgcontainer_GucefPubsubMsg)
  let root = if root == nil: cast[GucefPubsubMsgcontainer](this) else: cast[GucefPubsubMsgcontainer](root)
  this.io = io
  this.root = root
  this.parent = parent

  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 1)) != 0:
    block:
      let on = (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 2))
      if on == 0:
        let msgDateTimeExpr = uint64(this.io.readU4le())
        this.msgDateTime = msgDateTimeExpr
      else:
        let msgDateTimeExpr = this.io.readU8le()
        this.msgDateTime = msgDateTimeExpr
  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 3)) != 0:
    let msgidExpr = GucefCoreVariant.read(this.io, this.root, this)
    this.msgid = msgidExpr
  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 4)) != 0:
    let msgIndexExpr = GucefCoreVariant.read(this.io, this.root, this)
    this.msgIndex = msgIndexExpr
  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 5)) != 0:
    let primaryPayloadIncludedExpr = GucefCoreVariant.read(this.io, this.root, this)
    this.primaryPayloadIncluded = primaryPayloadIncludedExpr
  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 6)) != 0:
    let nrOfKvPairsExpr = this.io.readU4le()
    this.nrOfKvPairs = nrOfKvPairsExpr
  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 6)) != 0:
    for i in 0 ..< int(this.nrOfKvPairs):
      let it = GucefPubsubMsgcontainer_KvPair.read(this.io, this.root, this)
      this.keyValuePairs.add(it)
  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 7)) != 0:
    let nrOfMetadataKvPairsExpr = this.io.readU4le()
    this.nrOfMetadataKvPairs = nrOfMetadataKvPairsExpr
  if (GucefPubsubMsgcontainer(this.root).header.options and (1 shl 7)) != 0:
    for i in 0 ..< int(this.nrOfMetadataKvPairs):
      let it = GucefPubsubMsgcontainer_KvPair.read(this.io, this.root, this)
      this.metadataKeyValuePairs.add(it)

proc fromFile*(_: typedesc[GucefPubsubMsgcontainer_GucefPubsubMsg], filename: string): GucefPubsubMsgcontainer_GucefPubsubMsg =
  GucefPubsubMsgcontainer_GucefPubsubMsg.read(newKaitaiFileStream(filename), nil, nil)

proc read*(_: typedesc[GucefPubsubMsgcontainer_KvPair], io: KaitaiStream, root: KaitaiStruct, parent: GucefPubsubMsgcontainer_GucefPubsubMsg): GucefPubsubMsgcontainer_KvPair =
  template this: untyped = result
  this = new(GucefPubsubMsgcontainer_KvPair)
  let root = if root == nil: cast[GucefPubsubMsgcontainer](this) else: cast[GucefPubsubMsgcontainer](root)
  this.io = io
  this.root = root
  this.parent = parent

  let keyExpr = GucefCoreVariant.read(this.io, this.root, this)
  this.key = keyExpr
  let valueExpr = GucefCoreVariant.read(this.io, this.root, this)
  this.value = valueExpr

proc fromFile*(_: typedesc[GucefPubsubMsgcontainer_KvPair], filename: string): GucefPubsubMsgcontainer_KvPair =
  GucefPubsubMsgcontainer_KvPair.read(newKaitaiFileStream(filename), nil, nil)

