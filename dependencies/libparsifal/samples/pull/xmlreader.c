#include <stdlib.h>
#include <string.h>
#include "xmlreader.h"

#define ASSERT_MEM_ABORT(p) \
  if (!(p)) { return XML_ABORT; }

#define ASSERT_MEM_SETERR(p) \
  if (!(p)) { r->errorcode = ERR_XMLP_MEMORY_ALLOC; return NULL; }

int StartElement(struct XMLReader_t *r, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{
	union XMLReaderNode_t *rn = XMLVector_Append(r->eventbuffer, NULL);
	ASSERT_MEM_ABORT(rn);
	rn->type = XMLREADERTYPE_STARTELEMENT;
	
	ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[1], (XMLCH*)uri, rn->startelement.ibuf[0]));
	ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[1], (XMLCH*)localName, rn->startelement.ibuf[1]));
	ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[1], (XMLCH*)qName, rn->startelement.ibuf[2]));

	if (r->atts->length) ASSERT_MEM_ABORT(_XMLVector_RemoveAll(r->atts));

	if (atts->length) {
		struct XMLReaderAttribute_t *a;
		int i;		
		for (i=0; i<atts->length; i++) {
			LPXMLRUNTIMEATT aa = XMLVector_Get(atts, i);
			ASSERT_MEM_ABORT(a = XMLVector_Append(r->atts, NULL));
			ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[2], aa->uri, a->ibuf[0]));
			ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[2], aa->localName, a->ibuf[1]));
			ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[2], aa->qname, a->ibuf[2]));
			ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[2], aa->value, a->ibuf[3]));
		}
	}
	return XML_OK;
}

int EndElement(struct XMLReader_t *r, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{	
	union XMLReaderNode_t *rn = XMLVector_Append(r->eventbuffer, NULL);
	ASSERT_MEM_ABORT(rn);
	rn->type = XMLREADERTYPE_ENDELEMENT;

	ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[1], (XMLCH*)uri, rn->endelement.ibuf[0]));
	ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[1], (XMLCH*)localName, rn->endelement.ibuf[1]));
	ASSERT_MEM_ABORT(IBUF_PUT(&r->buf[1], (XMLCH*)qName, rn->endelement.ibuf[2]));
	return XML_OK;
}

static int CommonChars(struct XMLReader_t *r, int type, const XMLCH *chars, int cbSize)
{
	if (!r->buf[0].len) {
		union XMLReaderNode_t *rn = XMLVector_Append(r->eventbuffer, NULL);
		ASSERT_MEM_ABORT(rn);
		rn->type = type;
	}
	ASSERT_MEM_ABORT(XMLStringbuf_Append(&r->buf[0], (XMLCH*)chars, cbSize));
	return XML_OK;
}

static int Pi(struct XMLReader_t *r, const XMLCH *target, const XMLCH *data)
{	
	union XMLReaderNode_t *rn = XMLVector_Append(r->eventbuffer, NULL);
	int idata;
	ASSERT_MEM_ABORT(rn);
	rn->type = XMLREADERTYPE_PI;
	ASSERT_MEM_ABORT(XMLStringbuf_Append(&r->buf[0], (XMLCH*)target, strlen((const char*)target)+1));
	idata = r->buf[0].len;
	rn->pi.target = XMLStringbuf_Append(&r->buf[0], (XMLCH*)data, strlen((const char*)data)+1);
	ASSERT_MEM_ABORT(rn->pi.target);
	rn->pi.data = r->buf[0].str+idata;
	return XML_OK;
}

static void ErrorHandler(struct XMLReader_t *r) 
{ 
	if (r->parser->ErrorCode==ERR_XMLP_ABORT) /* we don't use ABORT for anything else but for */
		r->parser->ErrorCode = ERR_XMLP_MEMORY_ALLOC; /* out of memory */
	r->errorcode = r->parser->ErrorCode;
	if (r->errorHandler) r->errorHandler(r); /* delegate errorhandler cos GetSystemID etc.
											 will not be valid when errorhandler goes out of scope */
}

#define R3AD3R (struct XMLReader_t*)u
/* non-validating handlers: */
static int StartElementN(void *u, const XMLCH *i, const XMLCH *l, const XMLCH *n, LPXMLVECTOR a) 
{ return StartElement(R3AD3R,i,l,n,a); }
static int EndElementN(void *u, const XMLCH *i, const XMLCH *l, const XMLCH *n)
{ return EndElement(R3AD3R,i,l,n); }
static int CharsN(void *u, const XMLCH *c, int z) 
{ return CommonChars(R3AD3R,XMLREADERTYPE_CHARS,c,z); }
static int CommentN(void *u, const XMLCH *c, int z) 
{ return CommonChars(R3AD3R,XMLREADERTYPE_COMMENT,c,z); }
static int PiN(void *u, const XMLCH *t, const XMLCH *d)
{ return Pi(R3AD3R,t,d); }
static void ErrorHandlerN(LPXMLPARSER parser) 
{ ErrorHandler(((struct XMLReader_t*)parser->UserData)); }
#undef R3AD3R

#define R3AD3R (struct XMLReader_t*)((LPXMLDTDVALIDATOR)u)->UserData
/* validating handlers: */
static int StartElementV(void *u, const XMLCH *i, const XMLCH *l, const XMLCH *n, LPXMLVECTOR a) 
{ return StartElement(R3AD3R,i,l,n,a); }
static int EndElementV(void *u, const XMLCH *i, const XMLCH *l, const XMLCH *n)
{ return EndElement(R3AD3R,i,l,n); }
static int CharsV(void *u, const XMLCH *c, int z) 
{ return CommonChars(R3AD3R,XMLREADERTYPE_CHARS,c,z); }
static int CommentV(void *u, const XMLCH *c, int z) 
{ return CommonChars(R3AD3R,XMLREADERTYPE_COMMENT,c,z); }
static int PiV(void *u, const XMLCH *t, const XMLCH *d)
{ return Pi(R3AD3R,t,d); }
static void ErrorHandlerV(LPXMLPARSER parser) 
{ ErrorHandler((struct XMLReader_t*)((LPXMLDTDVALIDATOR)parser->UserData)->UserData); }
#undef R3AD3R

static int EndDocumentV(void *UserData)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	/* using validation with XMLFLAG_USE_SIMPLEPULL REQUIRES calling ParseValidateDTD(NULL)
	'cos otherwise validation will not be finished properly (no IDREF check etc). EndDocument is 
	good place for this call cos it is quaranteed to be called in the processing chain */
	XMLParser_ParseValidateDTD(v, v->parser, NULL, NULL, NULL);
	return XML_OK;
}

struct XMLReader_t *XMLReader_Create(struct XMLReader_t *r, int validating)
{	
	int i;
	if (!r) {
		r=malloc(sizeof(*r));
		ASSERT_MEM_SETERR(r);
		r->dynamicalloc = 1;
	}
	memset(r, 0, sizeof *r);
	ASSERT_MEM_SETERR(XMLParser_Create(&r->parser));
	XMLVector_Create(&r->atts, 0, sizeof(struct XMLReaderAttribute_t));
	ASSERT_MEM_SETERR(r->atts);
	XMLVector_Create(&r->eventbuffer, 0, sizeof(union XMLReaderNode_t));
	ASSERT_MEM_SETERR(r->eventbuffer);
	for (i=0; i<sizeof(r->buf)/sizeof(r->buf[0]); i++) {
		ASSERT_MEM_SETERR(XMLStringbuf_Init(&r->buf[i], 256, 256));
	}
	_XMLParser_SetFlag(r->parser, XMLFLAG_USE_SIMPLEPULL, 1);
	
	if (validating) {
		ASSERT_MEM_SETERR(r->dtdv = XMLParser_CreateDTDValidator());
		r->dtdv->UserData = r;
		r->parser->errorHandler = ErrorHandlerV;
		r->parser->startElementHandler = StartElementV;
		r->parser->endElementHandler = EndElementV;
		r->parser->charactersHandler = CharsV;
		r->parser->commentHandler = CommentV;
		r->parser->processingInstructionHandler = PiV;
		r->parser->endDocumentHandler = EndDocumentV;
	}
	else {
		r->parser->UserData = r;
		r->parser->errorHandler = ErrorHandlerN;		
		r->parser->startElementHandler = StartElementN;
		r->parser->endElementHandler = EndElementN;
		r->parser->charactersHandler = CharsN;
		r->parser->commentHandler = CommentN;
		r->parser->processingInstructionHandler = PiN;
	}
	return r;
}

void XMLReader_Free(struct XMLReader_t *r) 
{
	int i;
	if (!r) return;
	if (r->parser) XMLParser_Free(r->parser);
	if (r->atts) XMLVector_Free(r->atts);
	if (r->eventbuffer) XMLVector_Free(r->eventbuffer);
	for (i=0; i<sizeof(r->buf)/sizeof(r->buf[0]); i++) {
		XMLStringbuf_Free(&r->buf[i]);
	}
	if (r->dtdv) XMLParser_FreeDTDValidator(r->dtdv);
	if (r->dynamicalloc) free(r);
}

int XMLReader_Parse(struct XMLReader_t *r, LPFNINPUTSRC inputSrc, void *inputData, 
					const XMLCH *encoding)
{
	if (r->errorcode) r->errorcode=0;
	if (r->eventbuffer->length) /* clean up in the next _Read is necessary */
		r->eventbufferpos = r->eventbuffer->length;
	return (r->dtdv) ?
		XMLParser_ParseValidateDTD(r->dtdv, r->parser, inputSrc, inputData, encoding) :
		XMLParser_Parse(r->parser, inputSrc, inputData, encoding);
}

void XMLReader_AbortParse(struct XMLReader_t *r)
{	
	/* supress error reporting by removing errorHandler 
	before calling parse(NULL) so we don't get
	ERR_XMLP_UNCLOSED_TAG etc. errors. parser->ErrorCode
	will be set though */
	XML_ERROR_HANDLER h = r->parser->errorHandler;
	r->parser->errorHandler = NULL;
	XMLParser_Parse(r->parser, NULL, NULL, NULL);
	r->parser->errorHandler = h;
}

union XMLReaderNode_t *XMLReader_Read(struct XMLReader_t *r)
{		
	do {
		if (r->eventbuffer->length) {
			if (r->eventbufferpos == r->eventbuffer->length) {
				/* for (i=0; i<sizeof(r->buf)/sizeof(r->buf[0]); i++) */
				if (r->buf[0].len) XMLStringbuf_SetLength(&r->buf[0], 0);
				if (r->buf[1].len) XMLStringbuf_SetLength(&r->buf[1], 0);
				if (r->buf[2].len) XMLStringbuf_SetLength(&r->buf[2], 0);
				_XMLVector_RemoveAll(r->eventbuffer);
				r->eventbufferpos = 0;
			}
			else {
				union XMLReaderNode_t *rn = XMLVector_Get(r->eventbuffer, r->eventbufferpos);

				if (r->buf[0].len) {
					ASSERT_MEM_SETERR(XMLStringbuf_ToString(&r->buf[0]));
					if (rn->type == XMLREADERTYPE_CHARS) {
						rn->chars.text = r->buf[0].str;
						rn->chars.length = r->buf[0].len;
					}
					else if (rn->type == XMLREADERTYPE_COMMENT) {
						rn->comment.text = r->buf[0].str;
						rn->comment.length = r->buf[0].len;
					}
					r->buf[0].len = 0;
				}
				else {
					switch(rn->type) {
						case XMLREADERTYPE_STARTELEMENT:
							rn->startelement.uri = IBUF_GET(&r->buf[1], rn->startelement.ibuf[0]);
							rn->startelement.localname = IBUF_GET(&r->buf[1], rn->startelement.ibuf[1]);
							rn->startelement.qname = IBUF_GET(&r->buf[1], rn->startelement.ibuf[2]);
							rn->startelement.atts = r->atts;
						break;
						case XMLREADERTYPE_ENDELEMENT:
							rn->endelement.uri = IBUF_GET(&r->buf[1], rn->endelement.ibuf[0]);
							rn->endelement.localname = IBUF_GET(&r->buf[1], rn->endelement.ibuf[1]);
							rn->endelement.qname = IBUF_GET(&r->buf[1], rn->endelement.ibuf[2]);
						break;
					}
				}
				r->eventbufferpos++;
				return rn;
			}
		}
	} while (XMLParser_HasMoreEvents(r->parser));
		
	return NULL;
}
