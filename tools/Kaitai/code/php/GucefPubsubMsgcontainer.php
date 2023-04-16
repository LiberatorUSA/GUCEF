<?php
// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

namespace {
    class GucefPubsubMsgcontainer extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \Kaitai\Struct\Struct $_parent = null, \GucefPubsubMsgcontainer $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_header = new \GucefPubsubMsgcontainerHeader($this->_io);
            $this->_m_pubsubmsg = [];
            $i = 0;
            do {
                $_ = new \GucefPubsubMsgcontainer\GucefPubsubMsg($this->_io, $this, $this->_root);
                $this->_m_pubsubmsg[] = $_;
                $i++;
            } while (!($this->_io()->pos() == ((($this->_io()->size() - ($this->msgOffsetIdxSize() * 4)) - 4) - 10)));
            $this->_m_footer = new \GucefPubsubMsgcontainerFooter($this->_io);
        }
        protected $_m_msgOffsetIdxSize;
        public function msgOffsetIdxSize() {
            if ($this->_m_msgOffsetIdxSize !== null)
                return $this->_m_msgOffsetIdxSize;
            $_pos = $this->_io->pos();
            $this->_io->seek(($this->_io()->size() - 14));
            $this->_m_msgOffsetIdxSize = $this->_io->readU4le();
            $this->_io->seek($_pos);
            return $this->_m_msgOffsetIdxSize;
        }
        protected $_m_header;
        protected $_m_pubsubmsg;
        protected $_m_footer;
        public function header() { return $this->_m_header; }
        public function pubsubmsg() { return $this->_m_pubsubmsg; }
        public function footer() { return $this->_m_footer; }
    }
}

namespace GucefPubsubMsgcontainer {
    class GucefPubsubMsg extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefPubsubMsgcontainer $_parent = null, \GucefPubsubMsgcontainer $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            if (($this->_root()->header()->options() & (1 << 1)) != 0) {
                switch (($this->_root()->header()->options() & (1 << 2))) {
                    case 0:
                        $this->_m_msgDateTime = $this->_io->readU4le();
                        break;
                    default:
                        $this->_m_msgDateTime = $this->_io->readU8le();
                        break;
                }
            }
            if (($this->_root()->header()->options() & (1 << 3)) != 0) {
                $this->_m_msgid = new \GucefCoreVariant($this->_io);
            }
            if (($this->_root()->header()->options() & (1 << 4)) != 0) {
                $this->_m_msgIndex = new \GucefCoreVariant($this->_io);
            }
            if (($this->_root()->header()->options() & (1 << 5)) != 0) {
                $this->_m_primaryPayloadIncluded = new \GucefCoreVariant($this->_io);
            }
            if (($this->_root()->header()->options() & (1 << 6)) != 0) {
                $this->_m_nrOfKvPairs = $this->_io->readU4le();
            }
            if (($this->_root()->header()->options() & (1 << 6)) != 0) {
                $this->_m_keyValuePairs = [];
                $n = $this->nrOfKvPairs();
                for ($i = 0; $i < $n; $i++) {
                    $this->_m_keyValuePairs[] = new \GucefPubsubMsgcontainer\KvPair($this->_io, $this, $this->_root);
                }
            }
            if (($this->_root()->header()->options() & (1 << 7)) != 0) {
                $this->_m_nrOfMetadataKvPairs = $this->_io->readU4le();
            }
            if (($this->_root()->header()->options() & (1 << 7)) != 0) {
                $this->_m_metadataKeyValuePairs = [];
                $n = $this->nrOfMetadataKvPairs();
                for ($i = 0; $i < $n; $i++) {
                    $this->_m_metadataKeyValuePairs[] = new \GucefPubsubMsgcontainer\KvPair($this->_io, $this, $this->_root);
                }
            }
        }
        protected $_m_msgDateTime;
        protected $_m_msgid;
        protected $_m_msgIndex;
        protected $_m_primaryPayloadIncluded;
        protected $_m_nrOfKvPairs;
        protected $_m_keyValuePairs;
        protected $_m_nrOfMetadataKvPairs;
        protected $_m_metadataKeyValuePairs;
        public function msgDateTime() { return $this->_m_msgDateTime; }
        public function msgid() { return $this->_m_msgid; }
        public function msgIndex() { return $this->_m_msgIndex; }
        public function primaryPayloadIncluded() { return $this->_m_primaryPayloadIncluded; }
        public function nrOfKvPairs() { return $this->_m_nrOfKvPairs; }
        public function keyValuePairs() { return $this->_m_keyValuePairs; }
        public function nrOfMetadataKvPairs() { return $this->_m_nrOfMetadataKvPairs; }
        public function metadataKeyValuePairs() { return $this->_m_metadataKeyValuePairs; }
    }
}

namespace GucefPubsubMsgcontainer {
    class KvPair extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \GucefPubsubMsgcontainer\GucefPubsubMsg $_parent = null, \GucefPubsubMsgcontainer $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_key = new \GucefCoreVariant($this->_io);
            $this->_m_value = new \GucefCoreVariant($this->_io);
        }
        protected $_m_key;
        protected $_m_value;
        public function key() { return $this->_m_key; }
        public function value() { return $this->_m_value; }
    }
}
