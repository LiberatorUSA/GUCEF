// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "gucef_pubsub_msgcontainer_footer.h"
#include "kaitai/exceptions.h"

gucef_pubsub_msgcontainer_footer_t::gucef_pubsub_msgcontainer_footer_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, gucef_pubsub_msgcontainer_footer_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_msg_offset_idx = 0;
    f_mois = false;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void gucef_pubsub_msgcontainer_footer_t::_read() {
    m_msg_offset_idx = new std::vector<uint32_t>();
    const int l_msg_offset_idx = mois();
    for (int i = 0; i < l_msg_offset_idx; i++) {
        m_msg_offset_idx->push_back(m__io->read_u4le());
    }
    m_msg_offset_idx_size = m__io->read_u4le();
    m_magic_text = m__io->read_bytes(10);
    if (!(magic_text() == std::string("\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53", 10))) {
        throw kaitai::validation_not_equal_error<std::string>(std::string("\x50\x55\x42\x53\x55\x42\x4D\x53\x47\x53", 10), magic_text(), _io(), std::string("/seq/2"));
    }
}

gucef_pubsub_msgcontainer_footer_t::~gucef_pubsub_msgcontainer_footer_t() {
    _clean_up();
}

void gucef_pubsub_msgcontainer_footer_t::_clean_up() {
    if (m_msg_offset_idx) {
        delete m_msg_offset_idx; m_msg_offset_idx = 0;
    }
    if (f_mois) {
    }
}

uint32_t gucef_pubsub_msgcontainer_footer_t::mois() {
    if (f_mois)
        return m_mois;
    std::streampos _pos = m__io->pos();
    m__io->seek((_io()->size() - 14));
    m_mois = m__io->read_u4le();
    m__io->seek(_pos);
    f_mois = true;
    return m_mois;
}
