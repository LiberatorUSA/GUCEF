// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "gucef_pubsub_msgcontainer_header.h"
#include "kaitai/exceptions.h"

gucef_pubsub_msgcontainer_header_t::gucef_pubsub_msgcontainer_header_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, gucef_pubsub_msgcontainer_header_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void gucef_pubsub_msgcontainer_header_t::_read() {
    m_magic_text = m__io->read_bytes(10);
    if (!(magic_text() == std::string("\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53", 10))) {
        throw kaitai::validation_not_equal_error<std::string>(std::string("\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53", 10), magic_text(), _io(), std::string("/seq/0"));
    }
    m_version = m__io->read_u1();
    m_options = m__io->read_u4le();
}

gucef_pubsub_msgcontainer_header_t::~gucef_pubsub_msgcontainer_header_t() {
    _clean_up();
}

void gucef_pubsub_msgcontainer_header_t::_clean_up() {
}
