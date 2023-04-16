#ifndef GUCEF_PUBSUB_MSGCONTAINER_H_
#define GUCEF_PUBSUB_MSGCONTAINER_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include "gucef_pubsub_msgcontainer_footer.h"
#include "gucef_core_variant.h"
#include "gucef_pubsub_msgcontainer_header.h"
#include <vector>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif
class gucef_pubsub_msgcontainer_footer_t;
class gucef_core_variant_t;
class gucef_pubsub_msgcontainer_header_t;

class gucef_pubsub_msgcontainer_t : public kaitai::kstruct {

public:
    class gucef_pubsub_msg_t;
    class kv_pair_t;

    gucef_pubsub_msgcontainer_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = 0, gucef_pubsub_msgcontainer_t* p__root = 0);

private:
    void _read();
    void _clean_up();

public:
    ~gucef_pubsub_msgcontainer_t();

    class gucef_pubsub_msg_t : public kaitai::kstruct {

    public:

        gucef_pubsub_msg_t(kaitai::kstream* p__io, gucef_pubsub_msgcontainer_t* p__parent = 0, gucef_pubsub_msgcontainer_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~gucef_pubsub_msg_t();

    private:
        uint64_t m_msg_date_time;
        bool n_msg_date_time;

    public:
        bool _is_null_msg_date_time() { msg_date_time(); return n_msg_date_time; };

    private:
        gucef_core_variant_t* m_msgid;
        bool n_msgid;

    public:
        bool _is_null_msgid() { msgid(); return n_msgid; };

    private:
        gucef_core_variant_t* m_msg_index;
        bool n_msg_index;

    public:
        bool _is_null_msg_index() { msg_index(); return n_msg_index; };

    private:
        gucef_core_variant_t* m_primary_payload_included;
        bool n_primary_payload_included;

    public:
        bool _is_null_primary_payload_included() { primary_payload_included(); return n_primary_payload_included; };

    private:
        uint32_t m_nr_of_kv_pairs;
        bool n_nr_of_kv_pairs;

    public:
        bool _is_null_nr_of_kv_pairs() { nr_of_kv_pairs(); return n_nr_of_kv_pairs; };

    private:
        std::vector<kv_pair_t*>* m_key_value_pairs;
        bool n_key_value_pairs;

    public:
        bool _is_null_key_value_pairs() { key_value_pairs(); return n_key_value_pairs; };

    private:
        uint32_t m_nr_of_metadata_kv_pairs;
        bool n_nr_of_metadata_kv_pairs;

    public:
        bool _is_null_nr_of_metadata_kv_pairs() { nr_of_metadata_kv_pairs(); return n_nr_of_metadata_kv_pairs; };

    private:
        std::vector<kv_pair_t*>* m_metadata_key_value_pairs;
        bool n_metadata_key_value_pairs;

    public:
        bool _is_null_metadata_key_value_pairs() { metadata_key_value_pairs(); return n_metadata_key_value_pairs; };

    private:
        gucef_pubsub_msgcontainer_t* m__root;
        gucef_pubsub_msgcontainer_t* m__parent;

    public:
        uint64_t msg_date_time() const { return m_msg_date_time; }
        gucef_core_variant_t* msgid() const { return m_msgid; }
        gucef_core_variant_t* msg_index() const { return m_msg_index; }
        gucef_core_variant_t* primary_payload_included() const { return m_primary_payload_included; }
        uint32_t nr_of_kv_pairs() const { return m_nr_of_kv_pairs; }
        std::vector<kv_pair_t*>* key_value_pairs() const { return m_key_value_pairs; }
        uint32_t nr_of_metadata_kv_pairs() const { return m_nr_of_metadata_kv_pairs; }
        std::vector<kv_pair_t*>* metadata_key_value_pairs() const { return m_metadata_key_value_pairs; }
        gucef_pubsub_msgcontainer_t* _root() const { return m__root; }
        gucef_pubsub_msgcontainer_t* _parent() const { return m__parent; }
    };

    class kv_pair_t : public kaitai::kstruct {

    public:

        kv_pair_t(kaitai::kstream* p__io, gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t* p__parent = 0, gucef_pubsub_msgcontainer_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~kv_pair_t();

    private:
        gucef_core_variant_t* m_key;
        gucef_core_variant_t* m_value;
        gucef_pubsub_msgcontainer_t* m__root;
        gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t* m__parent;

    public:
        gucef_core_variant_t* key() const { return m_key; }
        gucef_core_variant_t* value() const { return m_value; }
        gucef_pubsub_msgcontainer_t* _root() const { return m__root; }
        gucef_pubsub_msgcontainer_t::gucef_pubsub_msg_t* _parent() const { return m__parent; }
    };

private:
    bool f_msg_offset_idx_size;
    uint32_t m_msg_offset_idx_size;

public:
    uint32_t msg_offset_idx_size();

private:
    gucef_pubsub_msgcontainer_header_t* m_header;
    std::vector<gucef_pubsub_msg_t*>* m_pubsubmsg;
    gucef_pubsub_msgcontainer_footer_t* m_footer;
    gucef_pubsub_msgcontainer_t* m__root;
    kaitai::kstruct* m__parent;

public:
    gucef_pubsub_msgcontainer_header_t* header() const { return m_header; }
    std::vector<gucef_pubsub_msg_t*>* pubsubmsg() const { return m_pubsubmsg; }
    gucef_pubsub_msgcontainer_footer_t* footer() const { return m_footer; }
    gucef_pubsub_msgcontainer_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // GUCEF_PUBSUB_MSGCONTAINER_H_
