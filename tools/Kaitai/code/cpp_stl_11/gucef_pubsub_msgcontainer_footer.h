#pragma once

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <memory>
#include <vector>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

class gucef_pubsub_msgcontainer_footer_t : public kaitai::kstruct {

public:

    gucef_pubsub_msgcontainer_footer_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = nullptr, gucef_pubsub_msgcontainer_footer_t* p__root = nullptr);

private:
    void _read();
    void _clean_up();

public:
    ~gucef_pubsub_msgcontainer_footer_t();

private:
    bool f_mois;
    uint32_t m_mois;

public:
    uint32_t mois();

private:
    std::unique_ptr<std::vector<uint32_t>> m_msg_offset_idx;
    uint32_t m_msg_offset_idx_size;
    std::string m_magic_text;
    gucef_pubsub_msgcontainer_footer_t* m__root;
    kaitai::kstruct* m__parent;

public:
    std::vector<uint32_t>* msg_offset_idx() const { return m_msg_offset_idx.get(); }
    uint32_t msg_offset_idx_size() const { return m_msg_offset_idx_size; }
    std::string magic_text() const { return m_magic_text; }
    gucef_pubsub_msgcontainer_footer_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};
