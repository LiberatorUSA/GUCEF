// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

use std::option::Option;
use std::boxed::Box;
use std::io::Result;
use std::io::Cursor;
use std::vec::Vec;
use std::default::Default;
use kaitai_struct::KaitaiStream;
use kaitai_struct::KaitaiStruct;
use gucef_core_dynamic::GucefCoreDynamic;

#[derive(Default)]
pub struct GucefCoreVariant {
    pub typeId: Box<GucefCoreVariant__GucefDatatypes>,
    pub payload: Option<Box<KaitaiStruct>>,
}

impl KaitaiStruct for GucefCoreVariant {
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
        self.typeId = self.stream.read_u1()?;
        match self.type_id {
            GucefCoreVariant__GucefDatatypes::DATETIME_ISO8601_ASCII_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UTF16_LE_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::INT16 => {
                self.payload = Box::new(GucefCoreVariant__GucefInt16::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::FLOAT32 => {
                self.payload = Box::new(GucefCoreVariant__GucefFloat32::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::ASCII_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UINT16 => {
                self.payload = Box::new(GucefCoreVariant__GucefUint16::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UINT64 => {
                self.payload = Box::new(GucefCoreVariant__GucefUint64::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::INT32 => {
                self.payload = Box::new(GucefCoreVariant__GucefInt32::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UINT8 => {
                self.payload = Box::new(GucefCoreVariant__GucefUint8::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UINT32 => {
                self.payload = Box::new(GucefCoreVariant__GucefUint32::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::FLOAT64 => {
                self.payload = Box::new(GucefCoreVariant__GucefFloat64::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH => {
                self.payload = Box::new(GucefCoreVariant__GucefBeTimestampInSecsSinceUnixEpoch::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH => {
                self.payload = Box::new(GucefCoreVariant__GucefBeTimestampInMsSinceUnixEpoch::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UTF32_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::BOOLEAN_UTF8_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::BINARY_BLOB => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH => {
                self.payload = Box::new(GucefCoreVariant__GucefLeTimestampInMsSinceUnixEpoch::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::BINARY_BSOB => {
                self.payload = Box::new(GucefCoreVariant__GucefBsob::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::BOOLEAN_ASCII_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UTF8_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::INT8 => {
                self.payload = Box::new(GucefCoreVariant__GucefInt8::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::BOOLEAN_INT32 => {
                self.payload = Box::new(GucefCoreVariant__GucefBooleanInt32::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH => {
                self.payload = Box::new(GucefCoreVariant__GucefLeTimestampInSecsSinceUnixEpoch::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::DATETIME_ISO8601_UTF8_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::INT64 => {
                self.payload = Box::new(GucefCoreVariant__GucefInt64::new(self.stream, self, _root)?);
            },
            GucefCoreVariant__GucefDatatypes::UTF16_BE_STRING => {
                self.payload = Box::new(GucefCoreDynamic::new(self.stream, self, _root)?);
            },
        }
    }
}

impl GucefCoreVariant {
}
enum GucefCoreVariant__GucefDatatypes {
    UINT8,
    INT8,
    UINT16,
    INT16,
    UINT32,
    INT32,
    UINT64,
    INT64,
    FLOAT32,
    FLOAT64,
    ASCII_STRING,
    UTF8_STRING,
    UTF16_LE_STRING,
    UTF16_BE_STRING,
    UTF32_STRING,
    BOOLEAN_INT32,
    BOOLEAN_ASCII_STRING,
    BOOLEAN_UTF8_STRING,
    BINARY_BLOB,
    BINARY_BSOB,
    DATETIME_ISO8601_ASCII_STRING,
    DATETIME_ISO8601_UTF8_STRING,
    LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH,
    BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH,
    LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH,
    BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH,
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefInt64 {
    pub data: i64,
}

impl KaitaiStruct for GucefCoreVariant__GucefInt64 {
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
        self.data = self.stream.read_s8le()?;
    }
}

impl GucefCoreVariant__GucefInt64 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefBsob {
    pub data: u8,
}

impl KaitaiStruct for GucefCoreVariant__GucefBsob {
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
        self.data = self.stream.read_u1()?;
    }
}

impl GucefCoreVariant__GucefBsob {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefBeTimestampInMsSinceUnixEpoch {
    pub data: u64,
}

impl KaitaiStruct for GucefCoreVariant__GucefBeTimestampInMsSinceUnixEpoch {
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
        self.data = self.stream.read_u8le()?;
    }
}

impl GucefCoreVariant__GucefBeTimestampInMsSinceUnixEpoch {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefLeTimestampInSecsSinceUnixEpoch {
    pub data: u64,
}

impl KaitaiStruct for GucefCoreVariant__GucefLeTimestampInSecsSinceUnixEpoch {
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
        self.data = self.stream.read_u8le()?;
    }
}

impl GucefCoreVariant__GucefLeTimestampInSecsSinceUnixEpoch {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefUint16 {
    pub data: u16,
}

impl KaitaiStruct for GucefCoreVariant__GucefUint16 {
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
        self.data = self.stream.read_u2le()?;
    }
}

impl GucefCoreVariant__GucefUint16 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefUint64 {
    pub data: u64,
}

impl KaitaiStruct for GucefCoreVariant__GucefUint64 {
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
        self.data = self.stream.read_u8le()?;
    }
}

impl GucefCoreVariant__GucefUint64 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefUint8 {
    pub data: u8,
}

impl KaitaiStruct for GucefCoreVariant__GucefUint8 {
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
        self.data = self.stream.read_u1()?;
    }
}

impl GucefCoreVariant__GucefUint8 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefInt8 {
    pub data: i8,
}

impl KaitaiStruct for GucefCoreVariant__GucefInt8 {
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
        self.data = self.stream.read_s1()?;
    }
}

impl GucefCoreVariant__GucefInt8 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefInt16 {
    pub data: i16,
}

impl KaitaiStruct for GucefCoreVariant__GucefInt16 {
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
        self.data = self.stream.read_s2le()?;
    }
}

impl GucefCoreVariant__GucefInt16 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefFloat64 {
    pub data: f64,
}

impl KaitaiStruct for GucefCoreVariant__GucefFloat64 {
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
        self.data = self.stream.read_f8le()?;
    }
}

impl GucefCoreVariant__GucefFloat64 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefLeTimestampInMsSinceUnixEpoch {
    pub data: u64,
}

impl KaitaiStruct for GucefCoreVariant__GucefLeTimestampInMsSinceUnixEpoch {
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
        self.data = self.stream.read_u8le()?;
    }
}

impl GucefCoreVariant__GucefLeTimestampInMsSinceUnixEpoch {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefInt32 {
    pub data: i32,
}

impl KaitaiStruct for GucefCoreVariant__GucefInt32 {
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
        self.data = self.stream.read_s4le()?;
    }
}

impl GucefCoreVariant__GucefInt32 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefBeTimestampInSecsSinceUnixEpoch {
    pub data: u64,
}

impl KaitaiStruct for GucefCoreVariant__GucefBeTimestampInSecsSinceUnixEpoch {
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
        self.data = self.stream.read_u8le()?;
    }
}

impl GucefCoreVariant__GucefBeTimestampInSecsSinceUnixEpoch {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefFloat32 {
    pub data: f32,
}

impl KaitaiStruct for GucefCoreVariant__GucefFloat32 {
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
        self.data = self.stream.read_f4le()?;
    }
}

impl GucefCoreVariant__GucefFloat32 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefUint32 {
    pub data: u32,
}

impl KaitaiStruct for GucefCoreVariant__GucefUint32 {
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
        self.data = self.stream.read_u4le()?;
    }
}

impl GucefCoreVariant__GucefUint32 {
}
#[derive(Default)]
pub struct GucefCoreVariant__GucefBooleanInt32 {
    pub data: i32,
}

impl KaitaiStruct for GucefCoreVariant__GucefBooleanInt32 {
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
        self.data = self.stream.read_s4le()?;
    }
}

impl GucefCoreVariant__GucefBooleanInt32 {
}
