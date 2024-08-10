#ifndef HEADER_CURL_CONFIG_H
#define HEADER_CURL_CONFIG_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2020, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

/* ================================================================ */
/*               Hand crafted config file for Linux                 */
/* ================================================================ */

/* Location of default ca bundle */
#define CURL_CA_BUNDLE "${CURL_CA_BUNDLE}"

/* define "1" to use built-in ca store of TLS backend */
#define CURL_CA_FALLBACK 1

/* Location of default ca path */
#define CURL_CA_PATH "${CURL_CA_PATH}"

/* Default SSL backend */
#define CURL_DEFAULT_SSL_BACKEND "${CURL_DEFAULT_SSL_BACKEND}"

/* disables alt-svc */
#define CURL_DISABLE_ALTSVC 1

/* disables cookies support */
#define CURL_DISABLE_COOKIES 1

/* disables Basic authentication */
#define CURL_DISABLE_BASIC_AUTH 1

/* disables Bearer authentication */
#define CURL_DISABLE_BEARER_AUTH 1

/* disables Digest authentication */
#define CURL_DISABLE_DIGEST_AUTH 1

/* disables Kerberos authentication */
#define CURL_DISABLE_KERBEROS_AUTH 1

/* disables negotiate authentication */
#define CURL_DISABLE_NEGOTIATE_AUTH 1

/* disables AWS-SIG4 */
#define CURL_DISABLE_AWS 1

/* disables DICT */
#define CURL_DISABLE_DICT 1

/* disables DNS-over-HTTPS */
#define CURL_DISABLE_DOH 1

/* disables FILE */
#define CURL_DISABLE_FILE 1

/* disables form api */
#define CURL_DISABLE_FORM_API 1

/* disables FTP */
#define CURL_DISABLE_FTP 1

/* disables curl_easy_options API for existing options to curl_easy_setopt */
#define CURL_DISABLE_GETOPTIONS 1

/* disables GOPHER */
#define CURL_DISABLE_GOPHER 1

/* disables headers-api support */
#define CURL_DISABLE_HEADERS_API 1

/* disables HSTS support */
#define CURL_DISABLE_HSTS 1

/* disables HTTP */
/* #define CURL_DISABLE_HTTP 1 */

/* disables IMAP */
#define CURL_DISABLE_IMAP 1

/* disables LDAP */
#define CURL_DISABLE_LDAP 1

/* disables LDAPS */
#define CURL_DISABLE_LDAPS 1

/* disables --libcurl option from the curl tool */
#define CURL_DISABLE_LIBCURL_OPTION 1

/* disables MIME support */
/* #define CURL_DISABLE_MIME 1 */

/* disables local binding support */
/* #define CURL_DISABLE_BINDLOCAL 1 */

/* disables MQTT */
#define CURL_DISABLE_MQTT 1

/* disables netrc parser */
#define CURL_DISABLE_NETRC 1

/* disables NTLM support */
#define CURL_DISABLE_NTLM 1

/* disables date parsing */
#define CURL_DISABLE_PARSEDATE 1

/* disables POP3 */
#define CURL_DISABLE_POP3 1

/* disables built-in progress meter */
#define CURL_DISABLE_PROGRESS_METER 1

/* disables proxies */
#define CURL_DISABLE_PROXY 1

/* disables RTSP */
#define CURL_DISABLE_RTSP 1

/* disables SMB */
#define CURL_DISABLE_SMB 1

/* disables SMTP */
#define CURL_DISABLE_SMTP 1

/* disables use of socketpair for curl_multi_poll */
#define CURL_DISABLE_SOCKETPAIR 1

/* disables TELNET */
#define CURL_DISABLE_TELNET 1

/* disables TFTP */
#define CURL_DISABLE_TFTP 1

/* disables verbose strings */
#define CURL_DISABLE_VERBOSE_STRINGS 1

/* to make a symbol visible */
#define CURL_EXTERN_SYMBOL ${CURL_EXTERN_SYMBOL}
/* Ensure using CURL_EXTERN_SYMBOL is possible */
#ifndef CURL_EXTERN_SYMBOL
#define CURL_EXTERN_SYMBOL
#endif

/* Allow SMB to work on Windows */
#define USE_WIN32_CRYPTO 1

/* Use Windows LDAP implementation */
#define USE_WIN32_LDAP 1

/* Define if you want to enable IPv6 support */
#define USE_IPV6 1

/* Define to 1 if you have the alarm function. */
#define HAVE_ALARM 1

/* Define to 1 if you have the arc4random function. */
#define HAVE_ARC4RANDOM 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have _Atomic support. */
#define HAVE_ATOMIC 1

/* Define to 1 if you have the `fnmatch' function. */
#define HAVE_FNMATCH 1

/* Define to 1 if you have the `basename' function. */
#define HAVE_BASENAME 1

/* Define to 1 if bool is an available type. */
#define HAVE_BOOL_T 1

/* Define to 1 if you have the __builtin_available function. */
#define HAVE_BUILTIN_AVAILABLE 1

/* Define to 1 if you have the clock_gettime function and monotonic timer. */
#define HAVE_CLOCK_GETTIME_MONOTONIC 1

/* Define to 1 if you have the clock_gettime function and raw monotonic timer.
   */
#define HAVE_CLOCK_GETTIME_MONOTONIC_RAW 1

/* Define to 1 if you have the `closesocket' function. */
#define HAVE_CLOSESOCKET 1

/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the `opendir' function. */
#define HAVE_OPENDIR 1

/* Define to 1 if you have the fcntl function. */
#define HAVE_FCNTL 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have a working fcntl O_NONBLOCK function. */
#define HAVE_FCNTL_O_NONBLOCK 1

/* Define to 1 if you have the freeaddrinfo function. */
#define HAVE_FREEADDRINFO 1

/* Define to 1 if you have the fseeko function. */
#define HAVE_FSEEKO 1

/* Define to 1 if you have the fseeko declaration. */
#define HAVE_DECL_FSEEKO 1

/* Define to 1 if you have the _fseeki64 function. */
#define HAVE__FSEEKI64 1

/* Define to 1 if you have the ftruncate function. */
#define HAVE_FTRUNCATE 1

/* Define to 1 if you have a working getaddrinfo function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if the getaddrinfo function is threadsafe. */
#define HAVE_GETADDRINFO_THREADSAFE 1

/* Define to 1 if you have the `geteuid' function. */
#define HAVE_GETEUID 1

/* Define to 1 if you have the `getppid' function. */
#define HAVE_GETPPID 1

/* Define to 1 if you have the gethostbyname_r function. */
#define HAVE_GETHOSTBYNAME_R 1

/* gethostbyname_r() takes 3 args */
#define HAVE_GETHOSTBYNAME_R_3 1

/* gethostbyname_r() takes 5 args */
#define HAVE_GETHOSTBYNAME_R_5 1

/* gethostbyname_r() takes 6 args */
#define HAVE_GETHOSTBYNAME_R_6 1

/* Define to 1 if you have the gethostname function. */
#define HAVE_GETHOSTNAME 1

/* Define to 1 if you have a working getifaddrs function. */
#define HAVE_GETIFADDRS 1

/* Define to 1 if you have the `getpass_r' function. */
#define HAVE_GETPASS_R 1

/* Define to 1 if you have the `getpeername' function. */
#define HAVE_GETPEERNAME 1

/* Define to 1 if you have the `getsockname' function. */
#define HAVE_GETSOCKNAME 1

/* Define to 1 if you have the `if_nametoindex' function. */
#define HAVE_IF_NAMETOINDEX 1

/* Define to 1 if you have the `getpwuid' function. */
#define HAVE_GETPWUID 1

/* Define to 1 if you have the `getpwuid_r' function. */
#define HAVE_GETPWUID_R 1

/* Define to 1 if you have the `getrlimit' function. */
#define HAVE_GETRLIMIT 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have a working glibc-style strerror_r function. */
#define HAVE_GLIBC_STRERROR_R 1

/* Define to 1 if you have a working gmtime_r function. */
#define HAVE_GMTIME_R 1

/* if you have the gssapi libraries */
#define HAVE_GSSAPI 1

/* Define to 1 if you have the <gssapi/gssapi_generic.h> header file. */
#define HAVE_GSSAPI_GSSAPI_GENERIC_H 1

/* Define to 1 if you have the <gssapi/gssapi.h> header file. */
#define HAVE_GSSAPI_GSSAPI_H 1

/* Define to 1 if you have the <gssapi/gssapi_krb5.h> header file. */
#define HAVE_GSSAPI_GSSAPI_KRB5_H 1

/* if you have the GNU gssapi libraries */
#define HAVE_GSSGNU 1

/* Define to 1 if you have the <ifaddrs.h> header file. */
#define HAVE_IFADDRS_H 1

/* Define to 1 if you have a IPv6 capable working inet_ntop function. */
#define HAVE_INET_NTOP 1

/* Define to 1 if you have a IPv6 capable working inet_pton function. */
#define HAVE_INET_PTON 1

/* Define to 1 if symbol `sa_family_t' exists */
#define HAVE_SA_FAMILY_T 1

/* Define to 1 if symbol `ADDRESS_FAMILY' exists */
#define HAVE_ADDRESS_FAMILY 1

/* Define to 1 if you have the ioctlsocket function. */
#define HAVE_IOCTLSOCKET 1

/* Define to 1 if you have the IoctlSocket camel case function. */
#define HAVE_IOCTLSOCKET_CAMEL 1

/* Define to 1 if you have a working IoctlSocket camel case FIONBIO function.
   */
#define HAVE_IOCTLSOCKET_CAMEL_FIONBIO 1

/* Define to 1 if you have a working ioctlsocket FIONBIO function. */
#define HAVE_IOCTLSOCKET_FIONBIO 1

/* Define to 1 if you have a working ioctl FIONBIO function. */
#define HAVE_IOCTL_FIONBIO 1

/* Define to 1 if you have a working ioctl SIOCGIFADDR function. */
#define HAVE_IOCTL_SIOCGIFADDR 1

/* Define to 1 if you have the <io.h> header file. */
#define HAVE_IO_H 1

/* Define to 1 if you have the lber.h header file. */
#define HAVE_LBER_H 1

/* Define to 1 if you have the ldap.h header file. */
#define HAVE_LDAP_H 1

/* Use LDAPS implementation */
#define HAVE_LDAP_SSL 1

/* Define to 1 if you have the ldap_ssl.h header file. */
#define HAVE_LDAP_SSL_H 1

/* Define to 1 if you have the `ldap_url_parse' function. */
#define HAVE_LDAP_URL_PARSE 1

/* Define to 1 if you have the <libgen.h> header file. */
#define HAVE_LIBGEN_H 1

/* Define to 1 if you have the `idn2' library (-lidn2). */
#define HAVE_LIBIDN2 1

/* Define to 1 if you have the idn2.h header file. */
#define HAVE_IDN2_H 1

/* if zlib is available */
#define HAVE_LIBZ 1

/* if brotli is available */
#define HAVE_BROTLI 1

/* if zstd is available */
#define HAVE_ZSTD 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if the compiler supports the 'long long' data type. */
#define HAVE_LONGLONG 1

/* Define to 1 if you have the 'suseconds_t' data type. */
#define HAVE_SUSECONDS_T 1

/* Define to 1 if you have the MSG_NOSIGNAL flag. */
#define HAVE_MSG_NOSIGNAL 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the <netinet/udp.h> header file. */
#define HAVE_NETINET_UDP_H 1

/* Define to 1 if you have the <linux/tcp.h> header file. */
#define HAVE_LINUX_TCP_H 1

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H 1

/* if you have an old MIT gssapi library, lacking GSS_C_NT_HOSTBASED_SERVICE */
#define HAVE_OLD_GSSMIT 1

/* Define to 1 if you have the `pipe' function. */
#define HAVE_PIPE 1

/* Define to 1 if you have the `eventfd' function. */
#define HAVE_EVENTFD 1

/* If you have a fine poll */
#define HAVE_POLL_FINE 1

/* Define to 1 if you have the <poll.h> header file. */
#define HAVE_POLL_H 1

/* Define to 1 if you have a working POSIX-style strerror_r function. */
#define HAVE_POSIX_STRERROR_R 1

/* Define to 1 if you have the <pthread.h> header file */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if OpenSSL has the `SSL_set0_wbio` function. */
#define HAVE_SSL_SET0_WBIO 1

/* Define to 1 if you have the recv function. */
#define HAVE_RECV 1

/* Define to 1 if you have the select function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the sched_yield function. */
#define HAVE_SCHED_YIELD 1

/* Define to 1 if you have the send function. */
#define HAVE_SEND 1

/* Define to 1 if you have the sendmsg function. */
#define HAVE_SENDMSG 1

/* Define to 1 if you have the 'fsetxattr' function. */
#define HAVE_FSETXATTR 1

/* fsetxattr() takes 5 args */
#define HAVE_FSETXATTR_5 1

/* fsetxattr() takes 6 args */
#define HAVE_FSETXATTR_6 1

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the `setmode' function. */
#define HAVE_SETMODE 1

/* Define to 1 if you have the `setrlimit' function. */
#define HAVE_SETRLIMIT 1

/* Define to 1 if you have a working setsockopt SO_NONBLOCK function. */
#define HAVE_SETSOCKOPT_SO_NONBLOCK 1

/* Define to 1 if you have the sigaction function. */
#define HAVE_SIGACTION 1

/* Define to 1 if you have the siginterrupt function. */
#define HAVE_SIGINTERRUPT 1

/* Define to 1 if you have the signal function. */
#define HAVE_SIGNAL 1

/* Define to 1 if you have the sigsetjmp function or macro. */
#define HAVE_SIGSETJMP 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if struct sockaddr_in6 has the sin6_scope_id member */
#define HAVE_SOCKADDR_IN6_SIN6_SCOPE_ID 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the socketpair function. */
#define HAVE_SOCKETPAIR 1

/* Define to 1 if you have the <stdatomic.h> header file. */
#define HAVE_STDATOMIC_H 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the strcasecmp function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the strcmpi function. */
#define HAVE_STRCMPI 1

/* Define to 1 if you have the strdup function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the strerror_r function. */
#define HAVE_STRERROR_R 1

/* Define to 1 if you have the stricmp function. */
#define HAVE_STRICMP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <stropts.h> header file. */
#define HAVE_STROPTS_H 1

/* Define to 1 if you have the strtok_r function. */
#define HAVE_STRTOK_R 1

/* Define to 1 if you have the strtoll function. */
#define HAVE_STRTOLL 1

/* Define to 1 if you have the memrchr function. */
#define HAVE_MEMRCHR 1

/* if struct sockaddr_storage is defined */
#define HAVE_STRUCT_SOCKADDR_STORAGE 1

/* Define to 1 if you have the timeval struct. */
#define HAVE_STRUCT_TIMEVAL 1

/* Define to 1 if you have the <sys/eventfd.h> header file. */
#define HAVE_SYS_EVENTFD_H 1

/* Define to 1 if you have the <sys/filio.h> header file. */
#define HAVE_SYS_FILIO_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
#define HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
#define HAVE_SYS_SOCKIO_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/un.h> header file. */
#define HAVE_SYS_UN_H 1

/* Define to 1 if you have the <sys/utime.h> header file. */
#define HAVE_SYS_UTIME_H 1

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <termio.h> header file. */
#define HAVE_TERMIO_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `utime' function. */
#define HAVE_UTIME 1

/* Define to 1 if you have the `utimes' function. */
#define HAVE_UTIMES 1

/* Define to 1 if you have the <utime.h> header file. */
#define HAVE_UTIME_H 1

/* Define this symbol if your OS supports changing the contents of argv */
#define HAVE_WRITABLE_ARGV 1

/* Define to 1 if you need the lber.h header file even with ldap.h */
#define NEED_LBER_H 1

/* Define to 1 if you need the malloc.h header file even with stdlib.h */
#define NEED_MALLOC_H 1

/* Define to 1 if _REENTRANT preprocessor symbol must be defined. */
#define NEED_REENTRANT 1

/* cpu-machine-OS */
#define OS ${OS}

/* Name of package */
#define PACKAGE ${PACKAGE}

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ${PACKAGE_BUGREPORT}

/* Define to the full name of this package. */
#define PACKAGE_NAME ${PACKAGE_NAME}

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ${PACKAGE_STRING}

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ${PACKAGE_TARNAME}

/* Define to the version of this package. */
#define PACKAGE_VERSION ${PACKAGE_VERSION}

/* a suitable file to read random data from */
#define RANDOM_FILE "${RANDOM_FILE}"

/*
 Note: SIZEOF_* variables are fetched with CMake through check_type_size().
 As per CMake documentation on CheckTypeSize, C preprocessor code is
 generated by CMake into SIZEOF_*_CODE. This is what we use in the
 following statements.

 Reference: https://cmake.org/cmake/help/latest/module/CheckTypeSize.html
*/

/* The size of `int', as computed by sizeof. /
${SIZEOF_INT_CODE}

/* The size of `long', as computed by sizeof. /
${SIZEOF_LONG_CODE}

/* The size of `long long', as computed by sizeof. /
${SIZEOF_LONG_LONG_CODE}

/* The size of `off_t', as computed by sizeof. /
${SIZEOF_OFF_T_CODE}

/* The size of `curl_off_t', as computed by sizeof. /
${SIZEOF_CURL_OFF_T_CODE}

/* The size of `curl_socket_t', as computed by sizeof. /
${SIZEOF_CURL_SOCKET_T_CODE}

/* The size of `size_t', as computed by sizeof. /
${SIZEOF_SIZE_T_CODE}

/* The size of `time_t', as computed by sizeof. /
${SIZEOF_TIME_T_CODE}

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if you want to enable c-ares support */
#define USE_ARES 1

/* Define if you want to enable POSIX threaded DNS lookup */
#define USE_THREADS_POSIX 1

/* Define if you want to enable Win32 threaded DNS lookup */
/* #define USE_THREADS_WIN32 1 */

/* if GnuTLS is enabled */
#define USE_GNUTLS 1

/* if Secure Transport is enabled */
#define USE_SECTRANSP 1

/* if mbedTLS is enabled */
#define USE_MBEDTLS 1

/* if BearSSL is enabled */
#define USE_BEARSSL 1

/* if wolfSSL is enabled */
#define USE_WOLFSSL 1

/* if wolfSSL has the wolfSSL_DES_ecb_encrypt function. */
#define HAVE_WOLFSSL_DES_ECB_ENCRYPT 1

/* if wolfSSL has the wolfSSL_BIO_set_shutdown function. */
#define HAVE_WOLFSSL_FULL_BIO 1

/* if libssh is in use */
#define USE_LIBSSH 1

/* if libssh2 is in use */
#define USE_LIBSSH2 1

/* if libpsl is in use */
#define USE_LIBPSL 1

/* if you want to use OpenLDAP code instead of legacy ldap implementation */
#define USE_OPENLDAP 1

/* if OpenSSL is in use */
#define USE_OPENSSL 1

/* if librtmp/rtmpdump is in use */
#define USE_LIBRTMP 1

/* if GSASL is in use */
#define USE_GSASL 1

/* if libuv is in use */
#define USE_LIBUV 1

/* Define to 1 if you have the <uv.h> header file. */
#define HAVE_UV_H 1

/* Define to 1 if you do not want the OpenSSL configuration to be loaded
   automatically */
#define CURL_DISABLE_OPENSSL_AUTO_LOAD_CONFIG 1

/* to enable NGHTTP2  */
#define USE_NGHTTP2 1

/* to enable NGTCP2 */
#define USE_NGTCP2 1

/* to enable NGHTTP3  */
#define USE_NGHTTP3 1

/* to enable quiche */
#define USE_QUICHE 1

/* to enable openssl + nghttp3 */
#define USE_OPENSSL_QUIC 1

/* Define to 1 if you have the quiche_conn_set_qlog_fd function. */
#define HAVE_QUICHE_CONN_SET_QLOG_FD 1

/* to enable msh3 */
#define USE_MSH3 1

/* if Unix domain sockets are enabled  */
#define USE_UNIX_SOCKETS 1

/* Define to 1 if you are building a Windows target with large file support. */
#define USE_WIN32_LARGE_FILES 1

/* to enable SSPI support */
#define USE_WINDOWS_SSPI 1

/* to enable Windows SSL  */
#define USE_SCHANNEL 1

/* enable multiple SSL backends */
#define CURL_WITH_MULTI_SSL 1

/* Version number of package */
#define VERSION ${VERSION}

/* Number of bits in a file offset, on hosts where this is settable. */
#define _FILE_OFFSET_BITS ${_FILE_OFFSET_BITS}

/* Define for large files, on AIX-style hosts. */
#define _LARGE_FILES ${_LARGE_FILES}

/* define this if you need it to compile thread-safe code */
#define _THREAD_SAFE ${_THREAD_SAFE}

/* Define to empty if `const' does not conform to ANSI C. */
#define const ${const}

/* Type to use in place of in_addr_t when system does not provide it. */
#define in_addr_t ${in_addr_t}

/* Define to `unsigned int' if <sys/types.h> does not define. */
#define size_t ${size_t}

/* the signed version of size_t */
#define ssize_t ${ssize_t}

/* Define to 1 if you have the mach_absolute_time function. */
#define HAVE_MACH_ABSOLUTE_TIME 1

/* to enable Windows IDN */
#define USE_WIN32_IDN 1

/* to enable Apple IDN */
#define USE_APPLE_IDN 1

/* Define to 1 to enable websocket support. */
#define USE_WEBSOCKETS 1

/* Define to 1 if OpenSSL has the SSL_CTX_set_srp_username function. */
#define HAVE_OPENSSL_SRP 1

/* Define to 1 if GnuTLS has the gnutls_srp_verifier function. */
#define HAVE_GNUTLS_SRP 1

/* Define to 1 to enable TLS-SRP support. */
#define USE_TLS_SRP 1

/* Define to 1 to query for HTTPSRR when using DoH */
#define USE_HTTPSRR 1

/* if ECH support is available */
#define USE_ECH 1

#endif /* HEADER_CURL_CONFIG_H */
