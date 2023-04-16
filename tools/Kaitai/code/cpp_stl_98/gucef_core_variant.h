#ifndef GUCEF_CORE_VARIANT_H_
#define GUCEF_CORE_VARIANT_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include "gucef_core_dynamic.h"

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif
class gucef_core_dynamic_t;

class gucef_core_variant_t : public kaitai::kstruct {

public:
    class gucef_int64_t;
    class gucef_bsob_t;
    class gucef_be_timestamp_in_ms_since_unix_epoch_t;
    class gucef_le_timestamp_in_secs_since_unix_epoch_t;
    class gucef_uint16_t;
    class gucef_uint64_t;
    class gucef_uint8_t;
    class gucef_int8_t;
    class gucef_int16_t;
    class gucef_float64_t;
    class gucef_le_timestamp_in_ms_since_unix_epoch_t;
    class gucef_int32_t;
    class gucef_be_timestamp_in_secs_since_unix_epoch_t;
    class gucef_float32_t;
    class gucef_uint32_t;
    class gucef_boolean_int32_t;

    enum gucef_datatypes_t {
        GUCEF_DATATYPES_UINT8 = 1,
        GUCEF_DATATYPES_INT8 = 2,
        GUCEF_DATATYPES_UINT16 = 3,
        GUCEF_DATATYPES_INT16 = 5,
        GUCEF_DATATYPES_UINT32 = 7,
        GUCEF_DATATYPES_INT32 = 9,
        GUCEF_DATATYPES_UINT64 = 11,
        GUCEF_DATATYPES_INT64 = 13,
        GUCEF_DATATYPES_FLOAT32 = 15,
        GUCEF_DATATYPES_FLOAT64 = 17,
        GUCEF_DATATYPES_ASCII_STRING = 21,
        GUCEF_DATATYPES_UTF8_STRING = 22,
        GUCEF_DATATYPES_UTF16_LE_STRING = 23,
        GUCEF_DATATYPES_UTF16_BE_STRING = 24,
        GUCEF_DATATYPES_UTF32_STRING = 25,
        GUCEF_DATATYPES_BOOLEAN_INT32 = 30,
        GUCEF_DATATYPES_BOOLEAN_ASCII_STRING = 31,
        GUCEF_DATATYPES_BOOLEAN_UTF8_STRING = 32,
        GUCEF_DATATYPES_BINARY_BLOB = 40,
        GUCEF_DATATYPES_BINARY_BSOB = 41,
        GUCEF_DATATYPES_DATETIME_ISO8601_ASCII_STRING = 50,
        GUCEF_DATATYPES_DATETIME_ISO8601_UTF8_STRING = 51,
        GUCEF_DATATYPES_LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH = 55,
        GUCEF_DATATYPES_BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH = 56,
        GUCEF_DATATYPES_LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH = 57,
        GUCEF_DATATYPES_BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH = 58
    };

    gucef_core_variant_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = 0, gucef_core_variant_t* p__root = 0);

private:
    void _read();
    void _clean_up();

public:
    ~gucef_core_variant_t();

    class gucef_int64_t : public kaitai::kstruct {

    public:

        gucef_int64_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_int64_t();

    private:
        int64_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        int64_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_bsob_t : public kaitai::kstruct {

    public:

        gucef_bsob_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_bsob_t();

    private:
        uint8_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint8_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_be_timestamp_in_ms_since_unix_epoch_t : public kaitai::kstruct {

    public:

        gucef_be_timestamp_in_ms_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_be_timestamp_in_ms_since_unix_epoch_t();

    private:
        uint64_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint64_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_le_timestamp_in_secs_since_unix_epoch_t : public kaitai::kstruct {

    public:

        gucef_le_timestamp_in_secs_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_le_timestamp_in_secs_since_unix_epoch_t();

    private:
        uint64_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint64_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_uint16_t : public kaitai::kstruct {

    public:

        gucef_uint16_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_uint16_t();

    private:
        uint16_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint16_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_uint64_t : public kaitai::kstruct {

    public:

        gucef_uint64_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_uint64_t();

    private:
        uint64_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint64_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_uint8_t : public kaitai::kstruct {

    public:

        gucef_uint8_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_uint8_t();

    private:
        uint8_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint8_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_int8_t : public kaitai::kstruct {

    public:

        gucef_int8_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_int8_t();

    private:
        int8_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        int8_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_int16_t : public kaitai::kstruct {

    public:

        gucef_int16_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_int16_t();

    private:
        int16_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        int16_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_float64_t : public kaitai::kstruct {

    public:

        gucef_float64_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_float64_t();

    private:
        double m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        double data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_le_timestamp_in_ms_since_unix_epoch_t : public kaitai::kstruct {

    public:

        gucef_le_timestamp_in_ms_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_le_timestamp_in_ms_since_unix_epoch_t();

    private:
        uint64_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint64_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_int32_t : public kaitai::kstruct {

    public:

        gucef_int32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_int32_t();

    private:
        int32_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        int32_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_be_timestamp_in_secs_since_unix_epoch_t : public kaitai::kstruct {

    public:

        gucef_be_timestamp_in_secs_since_unix_epoch_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_be_timestamp_in_secs_since_unix_epoch_t();

    private:
        uint64_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint64_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_float32_t : public kaitai::kstruct {

    public:

        gucef_float32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_float32_t();

    private:
        float m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        float data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_uint32_t : public kaitai::kstruct {

    public:

        gucef_uint32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_uint32_t();

    private:
        uint32_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        uint32_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

    class gucef_boolean_int32_t : public kaitai::kstruct {

    public:

        gucef_boolean_int32_t(kaitai::kstream* p__io, gucef_core_variant_t* p__parent = 0, gucef_core_variant_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_boolean_int32_t();

    private:
        int32_t m_data;
        gucef_core_variant_t* m__root;
        gucef_core_variant_t* m__parent;

    public:
        int32_t data() const { return m_data; }
        gucef_core_variant_t* _root() const { return m__root; }
        gucef_core_variant_t* _parent() const { return m__parent; }
    };

private:
    gucef_datatypes_t m_type_id;
    kaitai::kstruct* m_payload;
    bool n_payload;

public:
    bool _is_null_payload() { payload(); return n_payload; };

private:
    gucef_core_variant_t* m__root;
    kaitai::kstruct* m__parent;

public:
    gucef_datatypes_t type_id() const { return m_type_id; }
    kaitai::kstruct* payload() const { return m_payload; }
    gucef_core_variant_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // GUCEF_CORE_VARIANT_H_
