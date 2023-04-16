// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "gucef_pubsub_msgcontainer.h"

gucef_pubsub_msgcontainer_t::gucef_pubsub_msgcontainer_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, gucef_pubsub_msgcontainer_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_header = nullptr;
    m_pubsubmsg = nullptr;
    m_footer = nullptr;
    f_msg_offset_idx_size = false;
    _read();
}

void gucef_pubsub_msgcontainer_t::_read() {
    m_header = std::unique_ptr<gucef_pubsub_msgcontainer_header_t>(new gucef_pubsub_msgcontainer_header_t(m__io));
    m_pubsubmsg = std::unique_ptr<std::vector<std::unique_ptr<gucef_pubsub_msg_t>>>(new std::vector<std::unique_ptr<gucef_pubsub_msg_t>>());
    {
        int i = 0;
        gucef_pubsub_msg_t* _;
        do {
            _ = new gucef_pubsub_msg_t(m__io, this, m__root);
            m_pubsubmsg->push_back(std::move(std::unique_ptr<gucef_pubsub_msg_t>(_)));
            i++;
        } while (!(_io()->pos() == (((_io()->size() - (msg_offset_idx_size() * 4)) - 4) - 10)));
    }
    m_footer = std::unique_ptr<gucef_pubsub_msgcontainer_footer_t>(new gucef_pubsub_msgcontainer_footer_t(m__io));
}

gucef_pubsub_msgcontainer_t::~gucef_pubsub_msgcontainer_t() {
    _clean_up();
}

void gucef_pubsub_msgcontainer_t::_clean_up() {
    if (f_msg_offset_idx_size) {
    }
}

gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t::gucef_pubsub_msg_t(kaitai::kstream* p__io, gucef_pubsub_msgcontainer_t* p__parent, gucef_pubsub_msgcontainer_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_msgid = nullptr;
    m_msg_index = nullptr;
    m_primary_payload_included = nullptr;
    m_key_value_pairs = nullptr;
    m_metadata_key_value_pairs = nullptr;
    _read();
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
        m_msgid = std::unique_ptr<gucef_core_variant_t>(new gucef_core_variant_t(m__io));
    }
    n_msg_index = true;
    if ((_root()->header()->options() & (1 << 4)) != 0) {
        n_msg_index = false;
        m_msg_index = std::unique_ptr<gucef_core_variant_t>(new gucef_core_variant_t(m__io));
    }
    n_primary_payload_included = true;
    if ((_root()->header()->options() & (1 << 5)) != 0) {
        n_primary_payload_included = false;
        m_primary_payload_included = std::unique_ptr<gucef_core_variant_t>(new gucef_core_variant_t(m__io));
    }
    n_nr_of_kv_pairs = true;
    if ((_root()->header()->options() & (1 << 6)) != 0) {
        n_nr_of_kv_pairs = false;
        m_nr_of_kv_pairs = m__io->read_u4le();
    }
    n_key_value_pairs = true;
    if ((_root()->header()->options() & (1 << 6)) != 0) {
        n_key_value_pairs = false;
        m_key_value_pairs = std::unique_ptr<std::vector<std::unique_ptr<kv_pair_t>>>(new std::vector<std::unique_ptr<kv_pair_t>>());
        const int l_key_value_pairs = nr_of_kv_pairs();
        for (int i = 0; i < l_key_value_pairs; i++) {
            m_key_value_pairs->push_back(std::move(std::unique_ptr<kv_pair_t>(new kv_pair_t(m__io, this, m__root))));
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
        m_metadata_key_value_pairs = std::unique_ptr<std::vector<std::unique_ptr<kv_pair_t>>>(new std::vector<std::unique_ptr<kv_pair_t>>());
        const int l_metadata_key_value_pairs = nr_of_metadata_kv_pairs();
        for (int i = 0; i < l_metadata_key_value_pairs; i++) {
            m_metadata_key_value_pairs->push_back(std::move(std::unique_ptr<kv_pair_t>(new kv_pair_t(m__io, this, m__root))));
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
    }
    if (!n_msg_index) {
    }
    if (!n_primary_payload_included) {
    }
    if (!n_nr_of_kv_pairs) {
    }
    if (!n_key_value_pairs) {
    }
    if (!n_nr_of_metadata_kv_pairs) {
    }
    if (!n_metadata_key_value_pairs) {
    }
}

gucef_pubsub_msgcontainer_t::kv_pair_t::kv_pair_t(kaitai::kstream* p__io, gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t* p__parent, gucef_pubsub_msgcontainer_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_key = nullptr;
    m_value = nullptr;
    _read();
}

void gucef_pubsub_msgcontainer_t::kv_pair_t::_read() {
    m_key = std::unique_ptr<gucef_core_variant_t>(new gucef_core_variant_t(m__io));
    m_value = std::unique_ptr<gucef_core_variant_t>(new gucef_core_variant_t(m__io));
}

gucef_pubsub_msgcontainer_t::kv_pair_t::~kv_pair_t() {
    _clean_up();
}

void gucef_pubsub_msgcontainer_t::kv_pair_t::_clean_up() {
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
