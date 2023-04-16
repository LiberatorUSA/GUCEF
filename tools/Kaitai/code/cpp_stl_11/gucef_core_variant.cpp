// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "gucef_core_variant.h"

gucef_core_variant_t::gucef_core_variant_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    _read();
}

void gucef_core_variant_t::_read() {
    m_type_id = static_cast<gucef_core_variant_t::gucef_datatypes_t>(m__io->read_u1());
    n_payload = true;
    switch (type_id()) {
    case gucef_core_variant_t::GUCEF_DATATYPES_DATETIME_ISO8601_ASCII_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UTF16_LE_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_INT16: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_int16_t>(new gucef_int16_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_FLOAT32: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_float32_t>(new gucef_float32_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_ASCII_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UINT16: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_uint16_t>(new gucef_uint16_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UINT64: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_uint64_t>(new gucef_uint64_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_INT32: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_int32_t>(new gucef_int32_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UINT8: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_uint8_t>(new gucef_uint8_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UINT32: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_uint32_t>(new gucef_uint32_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_FLOAT64: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_float64_t>(new gucef_float64_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_be_timestamp_in_secs_since_unix_epoch_t>(new gucef_be_timestamp_in_secs_since_unix_epoch_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_be_timestamp_in_ms_since_unix_epoch_t>(new gucef_be_timestamp_in_ms_since_unix_epoch_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UTF32_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_BOOLEAN_UTF8_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_BINARY_BLOB: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_le_timestamp_in_ms_since_unix_epoch_t>(new gucef_le_timestamp_in_ms_since_unix_epoch_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_BINARY_BSOB: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_bsob_t>(new gucef_bsob_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_BOOLEAN_ASCII_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UTF8_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_INT8: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_int8_t>(new gucef_int8_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_BOOLEAN_INT32: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_boolean_int32_t>(new gucef_boolean_int32_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_le_timestamp_in_secs_since_unix_epoch_t>(new gucef_le_timestamp_in_secs_since_unix_epoch_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_DATETIME_ISO8601_UTF8_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_INT64: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_int64_t>(new gucef_int64_t(m__io, this, m__root));
        break;
    }
    case gucef_core_variant_t::GUCEF_DATATYPES_UTF16_BE_STRING: {
        n_payload = false;
        m_payload = std::unique_ptr<gucef_core_dynamic_t>(new gucef_core_dynamic_t(m__io));
        break;
    }
    }
}

gucef_core_variant_t::~gucef_core_variant_t() {
    _clean_up();
}

void gucef_core_variant_t::_clean_up() {
    if (!n_payload) {
    }
}

gucef_core_variant_t::gucef_int64_t::gucef_int64_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_int64_t::_read() {
    m_data = m__io->read_s8le();
}

gucef_core_variant_t::gucef_int64_t::~gucef_int64_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_int64_t::_clean_up() {
}

gucef_core_variant_t::gucef_bsob_t::gucef_bsob_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_bsob_t::_read() {
    m_data = m__io->read_u1();
}

gucef_core_variant_t::gucef_bsob_t::~gucef_bsob_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_bsob_t::_clean_up() {
}

gucef_core_variant_t::gucef_be_timestamp_in_ms_since_unix_epoch_t::gucef_be_timestamp_in_ms_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_be_timestamp_in_ms_since_unix_epoch_t::_read() {
    m_data = m__io->read_u8le();
}

gucef_core_variant_t::gucef_be_timestamp_in_ms_since_unix_epoch_t::~gucef_be_timestamp_in_ms_since_unix_epoch_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_be_timestamp_in_ms_since_unix_epoch_t::_clean_up() {
}

gucef_core_variant_t::gucef_le_timestamp_in_secs_since_unix_epoch_t::gucef_le_timestamp_in_secs_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_le_timestamp_in_secs_since_unix_epoch_t::_read() {
    m_data = m__io->read_u8le();
}

gucef_core_variant_t::gucef_le_timestamp_in_secs_since_unix_epoch_t::~gucef_le_timestamp_in_secs_since_unix_epoch_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_le_timestamp_in_secs_since_unix_epoch_t::_clean_up() {
}

gucef_core_variant_t::gucef_uint16_t::gucef_uint16_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_uint16_t::_read() {
    m_data = m__io->read_u2le();
}

gucef_core_variant_t::gucef_uint16_t::~gucef_uint16_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_uint16_t::_clean_up() {
}

gucef_core_variant_t::gucef_uint64_t::gucef_uint64_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_uint64_t::_read() {
    m_data = m__io->read_u8le();
}

gucef_core_variant_t::gucef_uint64_t::~gucef_uint64_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_uint64_t::_clean_up() {
}

gucef_core_variant_t::gucef_uint8_t::gucef_uint8_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_uint8_t::_read() {
    m_data = m__io->read_u1();
}

gucef_core_variant_t::gucef_uint8_t::~gucef_uint8_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_uint8_t::_clean_up() {
}

gucef_core_variant_t::gucef_int8_t::gucef_int8_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_int8_t::_read() {
    m_data = m__io->read_s1();
}

gucef_core_variant_t::gucef_int8_t::~gucef_int8_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_int8_t::_clean_up() {
}

gucef_core_variant_t::gucef_int16_t::gucef_int16_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_int16_t::_read() {
    m_data = m__io->read_s2le();
}

gucef_core_variant_t::gucef_int16_t::~gucef_int16_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_int16_t::_clean_up() {
}

gucef_core_variant_t::gucef_float64_t::gucef_float64_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_float64_t::_read() {
    m_data = m__io->read_f8le();
}

gucef_core_variant_t::gucef_float64_t::~gucef_float64_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_float64_t::_clean_up() {
}

gucef_core_variant_t::gucef_le_timestamp_in_ms_since_unix_epoch_t::gucef_le_timestamp_in_ms_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_le_timestamp_in_ms_since_unix_epoch_t::_read() {
    m_data = m__io->read_u8le();
}

gucef_core_variant_t::gucef_le_timestamp_in_ms_since_unix_epoch_t::~gucef_le_timestamp_in_ms_since_unix_epoch_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_le_timestamp_in_ms_since_unix_epoch_t::_clean_up() {
}

gucef_core_variant_t::gucef_int32_t::gucef_int32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_int32_t::_read() {
    m_data = m__io->read_s4le();
}

gucef_core_variant_t::gucef_int32_t::~gucef_int32_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_int32_t::_clean_up() {
}

gucef_core_variant_t::gucef_be_timestamp_in_secs_since_unix_epoch_t::gucef_be_timestamp_in_secs_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_be_timestamp_in_secs_since_unix_epoch_t::_read() {
    m_data = m__io->read_u8le();
}

gucef_core_variant_t::gucef_be_timestamp_in_secs_since_unix_epoch_t::~gucef_be_timestamp_in_secs_since_unix_epoch_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_be_timestamp_in_secs_since_unix_epoch_t::_clean_up() {
}

gucef_core_variant_t::gucef_float32_t::gucef_float32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_float32_t::_read() {
    m_data = m__io->read_f4le();
}

gucef_core_variant_t::gucef_float32_t::~gucef_float32_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_float32_t::_clean_up() {
}

gucef_core_variant_t::gucef_uint32_t::gucef_uint32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_uint32_t::_read() {
    m_data = m__io->read_u4le();
}

gucef_core_variant_t::gucef_uint32_t::~gucef_uint32_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_uint32_t::_clean_up() {
}

gucef_core_variant_t::gucef_boolean_int32_t::gucef_boolean_int32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent, gucef_core_variant_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void gucef_core_variant_t::gucef_boolean_int32_t::_read() {
    m_data = m__io->read_s4le();
}

gucef_core_variant_t::gucef_boolean_int32_t::~gucef_boolean_int32_t() {
    _clean_up();
}

void gucef_core_variant_t::gucef_boolean_int32_t::_clean_up() {
}
