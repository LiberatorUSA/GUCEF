<?php
// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

namespace {
    class GucefCoreVariant extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \Kaitai\Struct\Struct $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_typeId = $this->_io->readU1();
            switch ($this->typeId()) {
                case \GucefCoreVariant\GucefDatatypes::DATETIME_ISO8601_ASCII_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UTF16_LE_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::INT16:
                    $this->_m_payload = new \GucefCoreVariant\GucefInt16($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::FLOAT32:
                    $this->_m_payload = new \GucefCoreVariant\GucefFloat32($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::ASCII_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UINT16:
                    $this->_m_payload = new \GucefCoreVariant\GucefUint16($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UINT64:
                    $this->_m_payload = new \GucefCoreVariant\GucefUint64($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::INT32:
                    $this->_m_payload = new \GucefCoreVariant\GucefInt32($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UINT8:
                    $this->_m_payload = new \GucefCoreVariant\GucefUint8($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UINT32:
                    $this->_m_payload = new \GucefCoreVariant\GucefUint32($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::FLOAT64:
                    $this->_m_payload = new \GucefCoreVariant\GucefFloat64($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH:
                    $this->_m_payload = new \GucefCoreVariant\GucefBeTimestampInSecsSinceUnixEpoch($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH:
                    $this->_m_payload = new \GucefCoreVariant\GucefBeTimestampInMsSinceUnixEpoch($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UTF32_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::BOOLEAN_UTF8_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::BINARY_BLOB:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH:
                    $this->_m_payload = new \GucefCoreVariant\GucefLeTimestampInMsSinceUnixEpoch($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::BINARY_BSOB:
                    $this->_m_payload = new \GucefCoreVariant\GucefBsob($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::BOOLEAN_ASCII_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UTF8_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::INT8:
                    $this->_m_payload = new \GucefCoreVariant\GucefInt8($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::BOOLEAN_INT32:
                    $this->_m_payload = new \GucefCoreVariant\GucefBooleanInt32($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH:
                    $this->_m_payload = new \GucefCoreVariant\GucefLeTimestampInSecsSinceUnixEpoch($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::DATETIME_ISO8601_UTF8_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
                case \GucefCoreVariant\GucefDatatypes::INT64:
                    $this->_m_payload = new \GucefCoreVariant\GucefInt64($this->_io, $this, $this->_root);
                    break;
                case \GucefCoreVariant\GucefDatatypes::UTF16_BE_STRING:
                    $this->_m_payload = new \GucefCoreDynamic($this->_io);
                    break;
            }
        }
        protected $_m_typeId;
        protected $_m_payload;
        public function typeId() { return $this->_m_typeId; }
        public function payload() { return $this->_m_payload; }
    }
}

namespace GucefCoreVariant {
    class GucefInt64 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readS8le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefBsob extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU1();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefBeTimestampInMsSinceUnixEpoch extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU8le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefLeTimestampInSecsSinceUnixEpoch extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU8le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefUint16 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU2le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefUint64 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU8le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefUint8 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU1();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefInt8 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readS1();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefInt16 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readS2le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefFloat64 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readF8le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefLeTimestampInMsSinceUnixEpoch extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU8le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefInt32 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readS4le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefBeTimestampInSecsSinceUnixEpoch extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU8le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefFloat32 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readF4le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefUint32 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readU4le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefBooleanInt32 extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefCoreVariant $_parent = null, \GucefCoreVariant $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_data = $this->_io->readS4le();
        }
        protected $_m_data;
        public function data() { return $this->_m_data; }
    }
}

namespace GucefCoreVariant {
    class GucefDatatypes {
        const UINT8 = 1;
        const INT8 = 2;
        const UINT16 = 3;
        const INT16 = 5;
        const UINT32 = 7;
        const INT32 = 9;
        const UINT64 = 11;
        const INT64 = 13;
        const FLOAT32 = 15;
        const FLOAT64 = 17;
        const ASCII_STRING = 21;
        const UTF8_STRING = 22;
        const UTF16_LE_STRING = 23;
        const UTF16_BE_STRING = 24;
        const UTF32_STRING = 25;
        const BOOLEAN_INT32 = 30;
        const BOOLEAN_ASCII_STRING = 31;
        const BOOLEAN_UTF8_STRING = 32;
        const BINARY_BLOB = 40;
        const BINARY_BSOB = 41;
        const DATETIME_ISO8601_ASCII_STRING = 50;
        const DATETIME_ISO8601_UTF8_STRING = 51;
        const LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH = 55;
        const BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH = 56;
        const LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH = 57;
        const BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH = 58;
    }
}
