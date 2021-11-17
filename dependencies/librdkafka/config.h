#ifndef LIBRDKAFKA_CONFIG_H
#define LIBRDKAFKA_CONFIG_H

#define WITH_SSL 0
#define WITH_ZLIB 1
#define WITH_SNAPPY 0
#define WITH_ZSTD 0
/* zstd is linked dynamically on Windows, but the dynamic library provides
 * the experimental/advanced API, just as the static builds on *nix */
#define WITH_ZSTD_STATIC 0
#define WITH_SASL_SCRAM 0
#define WITH_SASL_OAUTHBEARER 0
#define ENABLE_DEVEL 0
#define WITH_PLUGINS 1
#define WITH_HDRHISTOGRAM 1

#define SOLIB_EXT ".so"

/* Notice: Keep up to date */
#define BUILT_WITH "SSL ZLIB SNAPPY SASL_SCRAM PLUGINS HDRHISTOGRAM"

#endif /* LIBRDKAFKA_CONFIG_H ? */
