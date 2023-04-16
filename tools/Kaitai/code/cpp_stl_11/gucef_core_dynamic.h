#pragma once

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <memory>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

class gucef_core_dynamic_t : public kaitai::kstruct {

public:

    gucef_core_dynamic_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = nullptr, gucef_core_dynamic_t* p__root = nullptr);

private:
    void _read();
    void _clean_up();

public:
    ~gucef_core_dynamic_t();

private:
    uint32_t m_len_data;
    std::string m_data;
    gucef_core_dynamic_t* m__root;
    kaitai::kstruct* m__parent;

public:
    uint32_t len_data() const { return m_len_data; }
    std::string data() const { return m_data; }
    gucef_core_dynamic_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};
