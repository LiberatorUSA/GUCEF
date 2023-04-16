<?php
// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

namespace {
    class GucefPubsubMsgcontainerFooter extends \Kaitai\Struct\Struct {
        public function __construct(\Kaitai\Struct\Stream $_io, \Kaitai\Struct\Struct $_parent = null, \GucefPubsubMsgcontainerFooter $_root = null) {
            parent::__construct($_io, $_parent, $_root);
            $this->_read();
        }

        private function _read() {
            $this->_m_msgOffsetIdx = [];
            $n = $this->mois();
            for ($i = 0; $i < $n; $i++) {
                $this->_m_msgOffsetIdx[] = $this->_io->readU4le();
            }
            $this->_m_msgOffsetIdxSize = $this->_io->readU4le();
            $this->_m_magicText = $this->_io->readBytes(10);
            if (!($this->magicText() == "\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53")) {
                throw new \Kaitai\Struct\Error\ValidationNotEqualError("\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53", $this->magicText(), $this->_io(), "/seq/2");
            }
        }
        protected $_m_mois;
        public function mois() {
            if ($this->_m_mois !== null)
                return $this->_m_mois;
            $_pos = $this->_io->pos();
            $this->_io->seek(($this->_io()->size() - 14));
            $this->_m_mois = $this->_io->readU4le();
            $this->_io->seek($_pos);
            return $this->_m_mois;
        }
        protected $_m_msgOffsetIdx;
        protected $_m_msgOffsetIdxSize;
        protected $_m_magicText;
        public function msgOffsetIdx() { return $this->_m_msgOffsetIdx; }
        public function msgOffsetIdxSize() { return $this->_m_msgOffsetIdxSize; }
        public function magicText() { return $this->_m_magicText; }
    }
}
