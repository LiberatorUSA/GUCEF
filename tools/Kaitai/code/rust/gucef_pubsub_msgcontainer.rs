// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

use std::option::Option;
use std::boxed::Box;
use std::io::Result;
use std::io::Cursor;
use std::vec::Vec;
use std::default::Default;
use kaitai_struct::KaitaiStream;
use kaitai_struct::KaitaiStruct;
use gucef_pubsub_msgcontainer_footer::GucefPubsubMsgcontainerFooter;
use gucef_core_variant::GucefCoreVariant;
use gucef_pubsub_msgcontainer_header::GucefPubsubMsgcontainerHeader;

#[derive(Default)]
pub struct GucefPubsubMsgcontainer {
    pub header: Box<GucefPubsubMsgcontainerHeader>,
    pub pubsubmsg: Vec<Box<GucefPubsubMsgcontainer__GucefPubsubMsg>>,
    pub footer: Box<GucefPubsubMsgcontainerFooter>,
    pub msgOffsetIdxSize: Option<u32>,
}

impl KaitaiStruct for GucefPubsubMsgcontainer {
    fn new<S: KaitaiStream>(stream: &mut S,
                            _parent: &Option<Box<KaitaiStruct>>,
                            _root: &Option<Box<KaitaiStruct>>)
                            -> Result<Self>
        where Self: Sized {
        let mut s: Self = Default::default();

        s.stream = stream;
        s.read(stream, _parent, _root)?;

        Ok(s)
    }


    fn read<S: KaitaiStream>(&mut self,
                             stream: &mut S,
                             _parent: &Option<Box<KaitaiStruct>>,
                             _root: &Option<Box<KaitaiStruct>>)
                             -> Result<()>
        where Self: Sized {
        self.header = Box::new(GucefPubsubMsgcontainerHeader::new(self.stream, self, _root)?);
        self.pubsubmsg = vec!();
        while {
            let tmpa = Box::new(GucefPubsubMsgcontainer__GucefPubsubMsg::new(self.stream, self, _root)?);
            self.pubsubmsg.append(tmpa);
            !(self._io.pos == (((self._io.size - (self.msg_offset_idx_size * 4)) - 4) - 10))
        } { }
        self.footer = Box::new(GucefPubsubMsgcontainerFooter::new(self.stream, self, _root)?);
    }
}

impl GucefPubsubMsgcontainer {
    fn msgOffsetIdxSize(&mut self) -> u32 {
        if let Some(x) = self.msgOffsetIdxSize {
            return x;
        }

        let _pos = self.stream.pos();
        self.stream.seek((self._io.size - 14));
        self.msgOffsetIdxSize = self.stream.read_u4le()?;
        self.stream.seek(_pos);
        return self.msgOffsetIdxSize;
    }
}
#[derive(Default)]
pub struct GucefPubsubMsgcontainer__GucefPubsubMsg {
    pub msgDateTime: u64,
    pub msgid: Box<GucefCoreVariant>,
    pub msgIndex: Box<GucefCoreVariant>,
    pub primaryPayloadIncluded: Box<GucefCoreVariant>,
    pub nrOfKvPairs: u32,
    pub keyValuePairs: Vec<Box<GucefPubsubMsgcontainer__KvPair>>,
    pub nrOfMetadataKvPairs: u32,
    pub metadataKeyValuePairs: Vec<Box<GucefPubsubMsgcontainer__KvPair>>,
}

impl KaitaiStruct for GucefPubsubMsgcontainer__GucefPubsubMsg {
    fn new<S: KaitaiStream>(stream: &mut S,
                            _parent: &Option<Box<KaitaiStruct>>,
                            _root: &Option<Box<KaitaiStruct>>)
                            -> Result<Self>
        where Self: Sized {
        let mut s: Self = Default::default();

        s.stream = stream;
        s.read(stream, _parent, _root)?;

        Ok(s)
    }


    fn read<S: KaitaiStream>(&mut self,
                             stream: &mut S,
                             _parent: &Option<Box<KaitaiStruct>>,
                             _root: &Option<Box<KaitaiStruct>>)
                             -> Result<()>
        where Self: Sized {
        if (self._root.header.options & (1 << 1)) != 0 {
            match (self._root.header.options & (1 << 2)) {
                0 => {
                    self.msgDateTime = self.stream.read_u4le()?;
                },
                _ => {
                    self.msgDateTime = self.stream.read_u8le()?;
                }
            }
        }
        if (self._root.header.options & (1 << 3)) != 0 {
            self.msgid = Box::new(GucefCoreVariant::new(self.stream, self, _root)?);
        }
        if (self._root.header.options & (1 << 4)) != 0 {
            self.msgIndex = Box::new(GucefCoreVariant::new(self.stream, self, _root)?);
        }
        if (self._root.header.options & (1 << 5)) != 0 {
            self.primaryPayloadIncluded = Box::new(GucefCoreVariant::new(self.stream, self, _root)?);
        }
        if (self._root.header.options & (1 << 6)) != 0 {
            self.nrOfKvPairs = self.stream.read_u4le()?;
        }
        if (self._root.header.options & (1 << 6)) != 0 {
            self.keyValuePairs = vec!();
            for i in 0..self.nr_of_kv_pairs {
                self.keyValuePairs.append(Box::new(GucefPubsubMsgcontainer__KvPair::new(self.stream, self, _root)?));
            }
        }
        if (self._root.header.options & (1 << 7)) != 0 {
            self.nrOfMetadataKvPairs = self.stream.read_u4le()?;
        }
        if (self._root.header.options & (1 << 7)) != 0 {
            self.metadataKeyValuePairs = vec!();
            for i in 0..self.nr_of_metadata_kv_pairs {
                self.metadataKeyValuePairs.append(Box::new(GucefPubsubMsgcontainer__KvPair::new(self.stream, self, _root)?));
            }
        }
    }
}

impl GucefPubsubMsgcontainer__GucefPubsubMsg {
}
#[derive(Default)]
pub struct GucefPubsubMsgcontainer__KvPair {
    pub key: Box<GucefCoreVariant>,
    pub value: Box<GucefCoreVariant>,
}

impl KaitaiStruct for GucefPubsubMsgcontainer__KvPair {
    fn new<S: KaitaiStream>(stream: &mut S,
                            _parent: &Option<Box<KaitaiStruct>>,
                            _root: &Option<Box<KaitaiStruct>>)
                            -> Result<Self>
        where Self: Sized {
        let mut s: Self = Default::default();

        s.stream = stream;
        s.read(stream, _parent, _root)?;

        Ok(s)
    }


    fn read<S: KaitaiStream>(&mut self,
                             stream: &mut S,
                             _parent: &Option<Box<KaitaiStruct>>,
                             _root: &Option<Box<KaitaiStruct>>)
                             -> Result<()>
        where Self: Sized {
        self.key = Box::new(GucefCoreVariant::new(self.stream, self, _root)?);
        self.value = Box::new(GucefCoreVariant::new(self.stream, self, _root)?);
    }
}

impl GucefPubsubMsgcontainer__KvPair {
}
