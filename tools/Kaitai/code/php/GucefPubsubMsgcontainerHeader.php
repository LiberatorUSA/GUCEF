<?php
// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

namespace {
    class GucefPubsubMsgcontainerHeader extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \Kaitai\Struct\Struct $_parent = null, \GucefPubsubMsgcontainerHeader $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_magicText = $this->_io->readBytes(10);
            if (!($this->magicText() == "\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53")) {
                throw new \Kaitai\Struct\Error\ValidationNotEqualError("\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53", $this->magicText(), $this->_io(), "/seq/0");
            }
            $this->_m_version = $this->_io->readU1();
            $this->_m_options = $this->_io->readU4le();
        }
        protected $_m_magicText;
        protected $_m_version;
        protected $_m_options;
        public function magicText() { return $this->_m_magicText; }
        public function version() { return $this->_m_version; }
        public function options() { return $this->_m_options; }
    }
}
