#ifndef XMLREADER__H
#define XMLREADER__H

#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"

enum XMLREADER_TYPES {
	XMLREADERTYPE_STARTELEMENT = 1,
	XMLREADERTYPE_ENDELEMENT,
	XMLREADERTYPE_CHARS,
	XMLREADERTYPE_COMMENT,
	XMLREADERTYPE_PI
};

union XMLReaderNode_t {
	int type;
	struct startelement {
		int type;
		const XMLCH *uri;
		const XMLCH *localname;
		const XMLCH *qname;
		int ibuf[3];
		LPXMLVECTOR atts;
	} startelement;
	struct endElement {
		int type;
		const XMLCH *uri;
		const XMLCH *localname;
		const XMLCH *qname;
		int ibuf[3];
	} endelement;
	struct chars {
		int type;
		XMLCH *text;
		int length;
	} chars;
	struct comment {
		int type;
		XMLCH *text;
		int length;
	} comment;
	struct pi {
		int type;
		XMLCH *target;
		XMLCH *data;
	} pi;
};

struct XMLReader_t {
	LPXMLPARSER parser;
	int eventbufferpos, dynamicalloc, errorcode;
	void *UserData;
	XMLSTRINGBUF buf[3];
	LPXMLVECTOR atts, eventbuffer;
	LPXMLDTDVALIDATOR dtdv;
	void (*errorHandler)(struct XMLReader_t *reader);
};

struct XMLReaderAttribute_t {
	int ibuf[4];
};

#define IBUF_PUT(sbuf, str, i) (!*str) ? (i = -1, str) : \
  (i = (sbuf)->len, XMLStringbuf_Append((sbuf), str, strlen((const char*)str)+1))
#define IBUF_GET(sbuf, i) (i==-1) ? (XMLCH*)"" : (sbuf)->str+i

#define XMLReader_GetAttributeUri(r, a) IBUF_GET(&(r)->buf[2], (a)->ibuf[0])
#define XMLReader_GetAttributeLocalName(r, a) IBUF_GET(&(r)->buf[2], (a)->ibuf[1])
#define XMLReader_GetAttributeQName(r, a) IBUF_GET(&(r)->buf[2], (a)->ibuf[2])
#define XMLReader_GetAttributeValue(r, a) IBUF_GET(&(r)->buf[2], (a)->ibuf[3])

struct XMLReader_t *XMLReader_Create(struct XMLReader_t *r, int validating);
void XMLReader_Free(struct XMLReader_t *r);
int XMLReader_Parse(struct XMLReader_t *r, LPFNINPUTSRC inputSrc, void *inputData, 
					const XMLCH *encoding);
void XMLReader_AbortParse(struct XMLReader_t *r);
union XMLReaderNode_t *XMLReader_Read(struct XMLReader_t *r);

#endif /* XMLREADER__H */
