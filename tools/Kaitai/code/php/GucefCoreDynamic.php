<?php
// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

namespace {
    class GucefCoreDynamic extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \Kaitai\Struct\Struct $_parent = null, \GucefCoreDynamic $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_payloadSize = $this->_io->readU4le();
            $this->_m_data = $this->_io->readBytes($this->payloadSize());
        }
        protected $_m_payloadSize;
        protected $_m_data;
        public function payloadSize() { return $this->_m_payloadSize; }
        public function data() { return $this->_m_data; }
    }
}
