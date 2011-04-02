/*===========================================================================
  Parsifal XML Parser
  Copyright (c) 2002-2008 Toni Uusitalo
  released to the public domain 2002-11-15
  http://www.saunalahti.fi/~samiuus/toni/xmlproc/

  Parsifal is free for both commercial and non-commercial use and
  redistribution, provided that author's copyright and disclaimer are
  retained intact.  You are free to modify Parsifal for your own use and
  to redistribute Parsifal with your modifications, provided that the
  modifications are clearly documented.

  DISCLAIMER
  ----------

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  Merchantability or fitness for a particular purpose. Please use it AT
  YOUR OWN RISK.
===========================================================================*/

#ifndef PARSIFAL__H
#define PARSIFAL__H

#ifdef __cplusplus
   extern "C" {
#endif

#include "pns.h"
#include "bistream.h"
#include "xmlhash.h"
#include "xmlvect.h"
#include "xmlsbuf.h"
#include "xmlpool.h"
#include "xmldtd.h"

/******************************************************************************/

/*
 *  Dinand Vanvelzen edit:
 *  Added this to build easily with CMake
 */
 
#ifdef PARSIFAL_CUSTOM_GUCEF_BUILD 

#ifndef XMLAPI
#define XMLAPI
#endif

#define PARSIFAL_XMLAPI XMLAPI
 
#if !( defined(PARSIFAL_LINUX_BUILD) || defined(PARSIFAL_MSWIN_BUILD) )
  #if defined( WIN32 ) || defined( _WIN32 )
    #define PARSIFAL_MSWIN_BUILD
  #elif defined( linux )
    #define PARSIFAL_LINUX_BUILD
  #else
    #error Cannot automaticly detect your operating system, please define
  #endif
#endif /* MANUAL_OS_DEFINE ? */ 
 
#ifdef PARSIFAL_MSWIN_BUILD
    #define PARSIFAL_EXPORT __declspec( dllexport )
    #define PARSIFAL_IMPORT __declspec( dllimport )
#else
    #define PARSIFAL_EXPORT
    #define PARSIFAL_IMPORT
#endif /* GUCEF_MSWIN_BUILD ? */

#undef XMLAPI
#ifdef PARSIFAL_BUILD_MODULE
  #define XMLAPI PARSIFAL_EXPORT
#else
  #define XMLAPI PARSIFAL_IMPORT
#endif

#endif /* PARSIFAL_CUSTOM_GUCEF_BUILD ? */

/******************************************************************************/

#ifndef XMLCH_DEFINED
    #define XMLCH_DEFINED
    typedef unsigned char XMLCH;
#endif

typedef struct tagXMLRUNTIMETAG
{
    XMLCH *qname;
    XMLCH *uri;
    XMLCH *localName;
    XMLCH *prefix;
    LPXMLHTABLE Scope;
    LPXMLHTABLE prevScope;
    XMLSTRINGBUF nameBuf;
} XMLRUNTIMETAG, *LPXMLRUNTIMETAG;

typedef struct tagXMLRUNTIMEATT
{
    XMLCH *qname;
    XMLCH *value;
    XMLCH *uri;
    XMLCH *localName;
    XMLCH *prefix;
    XMLSTRINGBUF nameBuf;
    XMLSTRINGBUF valBuf;
} XMLRUNTIMEATT, *LPXMLRUNTIMEATT;

enum tagXMLENTITYTYPE {
    XML_ENTITY_INT_PARAM = 1,
    XML_ENTITY_INT_GEN,
    XML_ENTITY_EXT_PARAM,
    XML_ENTITY_EXT_GEN,
    XML_ENTITY_UNPARSED,
    XML_ENTITY_DOCTYPE
};
typedef enum tagXMLENTITYTYPE XMLENTITYTYPE;

typedef struct tagXMLENTITY
{
    XMLENTITYTYPE type;
    int len;
    int open;
    XMLCH *name;
    XMLCH *value;
    XMLCH *publicID;
    XMLCH *systemID;
    XMLCH *notation;
} XMLENTITY, *LPXMLENTITY;

typedef struct tagXMLPARSERRUNTIME
{
    LPXMLHTABLE nsScope;
    LPXMLHTABLE namedAtts;
    LPXMLHTABLE entitiesTable;
    LPXMLHTABLE declAttTable;
    LPXMLHTABLE cpNames;
    LPXMLVECTOR atts;
    LPXMLVECTOR tagstack;
    LPXMLVECTOR entities;
    LPBUFFEREDISTREAM refReader;
    XMLCH *publicID, *systemID, *doctypeStr;
    const XMLCH *nameStart;
    XMLSTRINGBUF charsBuf;
    LPXMLPOOL strPool;
    LPXMLPOOL cpNodesPool;
    struct tagDTD *dtd;
    int doctypeParsed;
    XMLENTITY userSubset;
} XMLPARSERRUNTIME, *LPXMLPARSERRUNTIME;

enum tagXMLERRCODE {
    ERR_XMLP_MEMORY_ALLOC = 1,
    ERR_XMLP_READER_FATAL,
    ERR_XMLP_INVALID_TOKEN,
    ERR_XMLP_INVALID_NAME,
    ERR_XMLP_INVALID_END_TAG,
    ERR_XMLP_UNDEF_ENTITY,
    ERR_XMLP_WS_NOT_ALLOWED,
    ERR_XMLP_WS_REQUIRED,
    ERR_XMLP_UNCLOSED_TAG,
    ERR_XMLP_EXPECTED_FOUND,
    ERR_XMLP_EXPECTED_TOKEN,
    ERR_XMLP_MULTIPLE_TOP,
    ERR_XMLP_INVALID_AT_TOP,
    ERR_XMLP_UNDEF_NSPREFIX,
    ERR_XMLP_DUPL_ATTRIBUTE,
    ERR_XMLP_ENCODING,
    ERR_XMLP_UNSUP_ENCODING,
    ERR_XMLP_INVALID_DECL,
    ERR_XMLP_INVALID_ATT_VALUE,
    ERR_XMLP_ABORT,
    ERR_XMLP_ILLEGAL_CHAR,
    ERR_XMLP_RECURSIVE_ENTITY_REF,
    ERR_XMLP_IO,
    ERR_XMLP_SWITCH_ENCODING,
    ERR_XMLP_VALIDATION = 300
};
typedef enum tagXMLERRCODE XMLERRCODE;

#define XMLFLAG_NAMESPACES 0x1 /* http://xml.org/sax/features/namespaces */
#define XMLFLAG_NAMESPACE_PREFIXES 0x2 /* http://xml.org/sax/features/namespace-prefixes */
#define XMLFLAG_EXTERNAL_GENERAL_ENTITIES 0x4 /* http://xml.org/sax/features/external-general-entities */
#define XMLFLAG_PRESERVE_GENERAL_ENTITIES 0x8
#define XMLFLAG_UNDEF_GENERAL_ENTITIES 0x10
#define XMLFLAG_PRESERVE_WS_ATTRIBUTES 0x20
#define XMLFLAG_REPORT_DTD_EXT 0x40
#define XMLFLAG_VALIDATION_WARNINGS 0x80
#define XMLFLAG_SPLIT_LARGE_CONTENT 0x100
#define XMLFLAG_USE_SIMPLEPULL 0x200

typedef int (*XML_EVENT_HANDLER)(void *UserData);
typedef int (*XML_START_ELEMENT_HANDLER)(void *UserData, const XMLCH *uri,
                                         const XMLCH *localName, const XMLCH *qName,
                                         LPXMLVECTOR atts);
typedef int (*XML_END_ELEMENT_HANDLER)(void *UserData, const XMLCH *uri,
                                       const XMLCH *localName, const XMLCH *qName);
typedef int (*XML_CHARACTERS_HANDLER)(void *UserData, const XMLCH *chars, int cbSize);
typedef int (*XML_PI_HANDLER)(void *UserData, const XMLCH *target, const XMLCH *data);
typedef int (*XML_START_DTD_HANDLER)(void *UserData, const XMLCH *name,
                                     const XMLCH *publicId, const XMLCH *systemId,
                                     int hasInternalSubset);
typedef int (*XML_XMLDECL_HANDLER)(void *UserData, const XMLCH *version,
                                   const XMLCH *encoding, const XMLCH *standalone);
typedef int (*XML_RESOLVE_ENTITY_HANDLER)(void *UserData, LPXMLENTITY entity,
                                          LPBUFFEREDISTREAM reader);
typedef int (*XML_SKIPPED_ENTITY_HANDLER)(void *UserData, const XMLCH *name);
typedef int (*XML_ENTITY_EVENT_HANDLER)(void *UserData, LPXMLENTITY entity);
typedef int (*XML_ATTRIBUTEDECL_HANDLER)(void *UserData, const XMLCH *eName,
    const XMLCH *aName, int type, const XMLCH *typeStr, int valueDef,
    const XMLCH *def);
typedef int (*XML_ELEMENTDECL_HANDLER)(void *UserData, const XMLCH *name,
    void *contentModel);
typedef int (*XML_NOTATIONDECL_HANDLER)(void *UserData, const XMLCH *name,
    const XMLCH *publicID, const XMLCH *systemID);
typedef XMLCH* (*XML_ENCODINGALIAS_HANDLER)(void *UserData, const XMLCH *enc);

typedef struct tagXMLPARSER
{
    LPBUFFEREDISTREAM reader;
    LPXMLPARSERRUNTIME prt;
    XMLCH *DocumentElement;
    XMLCH ErrorString[128];
    int ErrorCode;
    int ErrorLine;
    int ErrorColumn;
    void *UserData;
    unsigned long XMLFlags;
    XML_EVENT_HANDLER startDocumentHandler;
    XML_EVENT_HANDLER endDocumentHandler;
    XML_EVENT_HANDLER startCDATAHandler;
    XML_EVENT_HANDLER endCDATAHandler;
    XML_EVENT_HANDLER endDTDHandler;
    XML_CHARACTERS_HANDLER charactersHandler;
    XML_CHARACTERS_HANDLER ignorableWhitespaceHandler;
    XML_CHARACTERS_HANDLER commentHandler;
    XML_ENCODINGALIAS_HANDLER encodingAliasHandler;
    XML_START_ELEMENT_HANDLER startElementHandler;
    XML_END_ELEMENT_HANDLER endElementHandler;
    XML_PI_HANDLER processingInstructionHandler;
    XML_START_DTD_HANDLER startDTDHandler;
    XML_XMLDECL_HANDLER xmlDeclHandler;
    XML_SKIPPED_ENTITY_HANDLER skippedEntityHandler;
    XML_ENTITY_EVENT_HANDLER startEntityHandler;
    XML_ENTITY_EVENT_HANDLER endEntityHandler;
    XML_RESOLVE_ENTITY_HANDLER resolveEntityHandler;
    XML_RESOLVE_ENTITY_HANDLER externalEntityParsedHandler;
    XML_ATTRIBUTEDECL_HANDLER attributeDeclHandler;
    XML_ELEMENTDECL_HANDLER elementDeclHandler;
    XML_ENTITY_EVENT_HANDLER entityDeclHandler;
    XML_NOTATIONDECL_HANDLER notationDeclHandler;
    /*XML_ERROR_HANDLER */ void (*errorHandler)(struct tagXMLPARSER *parser);
} XMLPARSER, *LPXMLPARSER;

typedef void (*XML_ERROR_HANDLER)(LPXMLPARSER parser);

#ifndef XMLAPI
#define XMLAPI
#endif

#define XML_OK 0
#define XML_ABORT 1

#define _XMLParser_SetFlag(parser,flag,valBool) \
  ((valBool) ? (((LPXMLPARSER)parser)->XMLFlags |= (flag)) : \
    (((LPXMLPARSER)parser)->XMLFlags &= ~(flag)) )

#define _XMLParser_GetFlag(parser,flag) \
  ((((LPXMLPARSER)parser)->XMLFlags & (flag)) == (flag))

#define _XMLParser_AttIsDefaulted(att) (!(att->nameBuf.str))

LPXMLPARSER XMLAPI XMLParser_Create(LPXMLPARSER *parser);
int XMLAPI XMLParser_Parse(LPXMLPARSER parser, LPFNINPUTSRC inputSrc, void *inputData, const XMLCH *encoding);
void XMLAPI XMLParser_Free(LPXMLPARSER parser);
LPXMLRUNTIMEATT XMLAPI XMLParser_GetNamedItem(LPXMLPARSER parser, const XMLCH *name);
XMLCH XMLAPI *XMLParser_GetSystemID(LPXMLPARSER parser);
XMLCH XMLAPI *XMLParser_GetPublicID(LPXMLPARSER parser);
XMLCH XMLAPI *XMLParser_GetPrefixMapping(LPXMLPARSER parser, const XMLCH *prefix);
int XMLAPI XMLParser_GetCurrentLine(LPXMLPARSER parser);
int XMLAPI XMLParser_GetCurrentColumn(LPXMLPARSER parser);
LPXMLENTITY XMLAPI XMLParser_GetCurrentEntity(LPXMLPARSER parser);
LPXMLENTITY XMLAPI XMLParser_SetExternalSubset(LPXMLPARSER parser, const XMLCH *publicID, const XMLCH *systemID);
XMLCH XMLAPI *XMLParser_GetVersionString(void);
int XMLAPI XMLParser_GetContextBytes(LPXMLPARSER parser, XMLCH **Bytes, int *cBytes);
int XMLAPI XMLNormalizeBuf(XMLCH *buf, int len);
int XMLAPI XMLIsNameStartChar(XMLCH *ch, int chSize);
int XMLAPI XMLIsNameChar(XMLCH *ch, int chSize);
int XMLAPI XMLParser_HasMoreEvents(LPXMLPARSER parser);

/******************************************************************************/

/*
 *  Dinand Vanvelzen edit:
 *  Added this to build easily with CMake
 */

#ifdef PARSIFAL_CUSTOM_GUCEF_BUILD 

#undef XMLAPI
#define XMLAPI PARSIFAL_XMLAPI

#endif /* PARSIFAL_CUSTOM_GUCEF_BUILD ? */
 
/******************************************************************************/


#ifdef __cplusplus
   }
#endif /* __cplusplus */
#endif /* PARSIFAL__H */


