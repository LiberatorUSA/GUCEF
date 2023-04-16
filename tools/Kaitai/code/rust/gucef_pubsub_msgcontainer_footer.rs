// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

use std::option::Option;
use std::boxed::Box;
use std::io::Result;
use std::io::Cursor;
use std::vec::Vec;
use std::default::Default;
use kaitai_struct::KaitaiStream;
use kaitai_struct::KaitaiStruct;

#[derive(Default)]
pub struct GucefPubsubMsgcontainerFooter {
    pub msgOffsetIdx: Vec<u32>,
    pub msgOffsetIdxSize: u32,
    pub magicText: Vec<u8>,
    pub mois: Option<u32>,
}

impl KaitaiStruct for GucefPubsubMsgcontainerFooter {
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
        self.msgOffsetIdx = vec!();
        for i in 0..self.mois {
            self.msgOffsetIdx.append(self.stream.read_u4le()?);
        }
        self.msgOffsetIdxSize = self.stream.read_u4le()?;
        self.magicText = self.stream.read_bytes(10)?;
    }
}

impl GucefPubsubMsgcontainerFooter {
    fn mois(&mut self) -> u32 {
        if let Some(x) = self.mois {
            return x;
        }

        let _pos = self.stream.pos();
        self.stream.seek((self._io.size - 14));
        self.mois = self.stream.read_u4le()?;
        self.stream.seek(_pos);
        return self.mois;
    }
}
