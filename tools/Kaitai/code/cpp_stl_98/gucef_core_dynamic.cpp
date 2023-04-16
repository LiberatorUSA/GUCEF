// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "gucef_core_dynamic.h"

gucef_core_dynamic_t::gucef_core_dynamic_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, gucef_core_dynamic_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void gucef_core_dynamic_t::_read() {
    m_payload_size = m__io->read_u4le();
    m_data = m__io->read_bytes(payload_size());
}

gucef_core_dynamic_t::~gucef_core_dynamic_t() {
    _clean_up();
}

void gucef_core_dynamic_t::_clean_up() {
}
