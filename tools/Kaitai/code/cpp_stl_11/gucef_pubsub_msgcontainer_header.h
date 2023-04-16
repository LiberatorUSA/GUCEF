#pragma once

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <memory>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

class gucef_pubsub_msgcontainer_header_t : public kaitai::kstruct {

public:

    gucef_pubsub_msgcontainer_header_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = nullptr, gucef_pubsub_msgcontainer_header_t* p__root = nullptr);

private:
    void _read();
    void _clean_up();

public:
    ~gucef_pubsub_msgcontainer_header_t();

private:
    std::string m_magic_text;
    uint8_t m_version;
    uint32_t m_options;
    gucef_pubsub_msgcontainer_header_t* m__root;
    kaitai::kstruct* m__parent;

public:
    std::string magic_text() const { return m_magic_text; }
    uint8_t version() const { return m_version; }
    uint32_t options() const { return m_options; }
    gucef_pubsub_msgcontainer_header_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};
