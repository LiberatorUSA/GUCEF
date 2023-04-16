// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "gucef_pubsub_msgcontainer.h"

gucef_pubsub_msgcontainer_t::gucef_pubsub_msgcontainer_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, gucef_pubsub_msgcontainer_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_header = 0;
    m_pubsubmsg = 0;
    m_footer = 0;
    f_msg_offset_idx_size = false;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void gucef_pubsub_msgcontainer_t::_read() {
    m_header = new gucef_pubsub_msgcontainer_header_t(m__io);
    m_pubsubmsg = new std::vector<gucef_pubsub_msg_t*>();
    {
        int i = 0;
        gucef_pubsub_msg_t* _;
        do {
            _ = new gucef_pubsub_msg_t(m__io, this, m__root);
            m_pubsubmsg->push_back(_);
            i++;
        } while (!(_io()->pos() == (((_io()->size() - (msg_offset_idx_size() * 4)) - 4) - 10)));
    }
    m_footer = new gucef_pubsub_msgcontainer_footer_t(m__io);
}

gucef_pubsub_msgcontainer_t::~gucef_pubsub_msgcontainer_t() {
    _clean_up();
}

void gucef_pubsub_msgcontainer_t::_clean_up() {
    if (m_header) {
        delete m_header; m_header = 0;
    }
    if (m_pubsubmsg) {
        for (std::vector<gucef_pubsub_msg_t*>::iterator it = m_pubsubmsg->begin(); it != m_pubsubmsg->end(); ++it) {
            delete *it;
        }
        delete m_pubsubmsg; m_pubsubmsg = 0;
    }
    if (m_footer) {
        delete m_footer; m_footer = 0;
    }
    if (f_msg_offset_idx_size) {
    }
}

gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t::gucef_pubsub_msg_t(kaitai::kstream* p__io, gucef_pubsub_msgcontainer_t* p__parent, gucef_pubsub_msgcontainer_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_msgid = 0;
    m_msg_index = 0;
    m_primary_payload_included = 0;
    m_key_value_pairs = 0;
    m_metadata_key_value_pairs = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t::_read() {
    n_msg_date_time = true;
    if ((_root()->header()->options() & (1 << 1)) != 0) {
        n_msg_date_time = false;
        switch ((_root()->header()->options() & (1 << 2))) {
        case 0: {
            m_msg_date_time = m__io->read_u4le();
            break;
        }
        default: {
            m_msg_date_time = m__io->read_u8le();
            break;
        }
        }
    }
    n_msgid = true;
    if ((_root()->header()->options() & (1 << 3)) != 0) {
        n_msgid = false;
        m_msgid = new gucef_core_variant_t(m__io);
    }
    n_msg_index = true;
    if ((_root()->header()->options() & (1 << 4)) != 0) {
        n_msg_index = false;
        m_msg_index = new gucef_core_variant_t(m__io);
    }
    n_primary_payload_included = true;
    if ((_root()->header()->options() & (1 << 5)) != 0) {
        n_primary_payload_included = false;
        m_primary_payload_included = new gucef_core_variant_t(m__io);
    }
    n_nr_of_kv_pairs = true;
    if ((_root()->header()->options() & (1 << 6)) != 0) {
        n_nr_of_kv_pairs = false;
        m_nr_of_kv_pairs = m__io->read_u4le();
    }
    n_key_value_pairs = true;
    if ((_root()->header()->options() & (1 << 6)) != 0) {
        n_key_value_pairs = false;
        m_key_value_pairs = new std::vector<kv_pair_t*>();
        const int l_key_value_pairs = nr_of_kv_pairs();
        for (int i = 0; i < l_key_value_pairs; i++) {
            m_key_value_pairs->push_back(new kv_pair_t(m__io, this, m__root));
        }
    }
    n_nr_of_metadata_kv_pairs = true;
    if ((_root()->header()->options() & (1 << 7)) != 0) {
        n_nr_of_metadata_kv_pairs = false;
        m_nr_of_metadata_kv_pairs = m__io->read_u4le();
    }
    n_metadata_key_value_pairs = true;
    if ((_root()->header()->options() & (1 << 7)) != 0) {
        n_metadata_key_value_pairs = false;
        m_metadata_key_value_pairs = new std::vector<kv_pair_t*>();
        const int l_metadata_key_value_pairs = nr_of_metadata_kv_pairs();
        for (int i = 0; i < l_metadata_key_value_pairs; i++) {
            m_metadata_key_value_pairs->push_back(new kv_pair_t(m__io, this, m__root));
        }
    }
}

gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t::~gucef_pubsub_msg_t() {
    _clean_up();
}

void gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t::_clean_up() {
    if (!n_msg_date_time) {
    }
    if (!n_msgid) {
        if (m_msgid) {
            delete m_msgid; m_msgid = 0;
        }
    }
    if (!n_msg_index) {
        if (m_msg_index) {
            delete m_msg_index; m_msg_index = 0;
        }
    }
    if (!n_primary_payload_included) {
        if (m_primary_payload_included) {
            delete m_primary_payload_included; m_primary_payload_included = 0;
        }
    }
    if (!n_nr_of_kv_pairs) {
    }
    if (!n_key_value_pairs) {
        if (m_key_value_pairs) {
            for (std::vector<kv_pair_t*>::iterator it = m_key_value_pairs->begin(); it != m_key_value_pairs->end(); ++it) {
                delete *it;
            }
            delete m_key_value_pairs; m_key_value_pairs = 0;
        }
    }
    if (!n_nr_of_metadata_kv_pairs) {
    }
    if (!n_metadata_key_value_pairs) {
        if (m_metadata_key_value_pairs) {
            for (std::vector<kv_pair_t*>::iterator it = m_metadata_key_value_pairs->begin(); it != m_metadata_key_value_pairs->end(); ++it) {
                delete *it;
            }
            delete m_metadata_key_value_pairs; m_metadata_key_value_pairs = 0;
        }
    }
}

gucef_pubsub_msgcontainer_t::kv_pair_t::kv_pair_t(kaitai::kstream* p__io, gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t* p__parent, gucef_pubsub_msgcontainer_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_key = 0;
    m_value = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void gucef_pubsub_msgcontainer_t::kv_pair_t::_read() {
    m_key = new gucef_core_variant_t(m__io);
    m_value = new gucef_core_variant_t(m__io);
}

gucef_pubsub_msgcontainer_t::kv_pair_t::~kv_pair_t() {
    _clean_up();
}

void gucef_pubsub_msgcontainer_t::kv_pair_t::_clean_up() {
    if (m_key) {
        delete m_key; m_key = 0;
    }
    if (m_value) {
        delete m_value; m_value = 0;
    }
}

uint32_t gucef_pubsub_msgcontainer_t::msg_offset_idx_size() {
    if (f_msg_offset_idx_size)
        return m_msg_offset_idx_size;
    std::streampos _pos = m__io->pos();
    m__io->seek((_io()->size() - 14));
    m_msg_offset_idx_size = m__io->read_u4le();
    m__io->seek(_pos);
    f_msg_offset_idx_size = true;
    return m_msg_offset_idx_size;
}
