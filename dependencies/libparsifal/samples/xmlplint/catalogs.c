/* Basic implementation of XML catalogs for Parsifal XML Parser
   http://www.oasis-open.org/committees/entity/spec-2001-08-06.html
   see also http://www.xmlsoft.org/catalog.html
   Currently support only publicID/systemID mappings + xml:base + 
   nextCatalog. Also catalog files/external entities must be local files.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"
#include "uriresolver.h"

#ifndef ASSERT_MEM_ABORT
#define ASSERT_MEM_ABORT(p) \
  if (!(p)) { printf("Out of memory! Line: %d\n", __LINE__); return XML_ABORT; }
#endif

struct IDs {
	XMLCH *id;
	XMLCH *uri;
};

extern char *AppBase;
extern XMLSTRINGBUF uribuf;
extern int filestream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);
extern FILE *mfopen(const char *n, const char *m);

static LPXMLPARSER parser;
static LPXMLVECTOR publicIDS, systemIDS, nextCatalog;
static XMLCH *doc;
static struct uriresolver_input_t *curinput;

static int idsort(const void *ID1, const void *ID2)
{
	return strcmp(((struct IDs *)ID1)->id, ((struct IDs *)ID2)->id);
}

static int idsearch(const void *key, const void *ID)
{
	return strcmp((XMLCH*)key, ((struct IDs *)ID)->id);
}

static int GetUriAndBase(LPXMLRUNTIMEATT a, XMLCH **target)
{
	XMLCH *baseuri = XMLParser_GetPrefixMapping(parser, "xml:base");
	if (baseuri) {
		int l1 = strlen(baseuri);
		int l2 = strlen(a->value);
		int len = l1+l2+1;
		*target = malloc(len);
		ASSERT_MEM_ABORT(*target);
		memcpy(*target, baseuri, l1);
		memcpy((*target)+l1, a->value, l2);
		(*target)[len-1]=0;
	}
	else {
		*target = strdup(a->value);
		ASSERT_MEM_ABORT(*target);
	}
	return 0;
}

static int AddUri(LPXMLVECTOR *v, LPXMLRUNTIMEATT idAtt)
{
	struct IDs *id;
	LPXMLRUNTIMEATT a;
		
	ASSERT_MEM_ABORT(idAtt); /* shouldn't happen */
	if (!*v) {
		XMLVector_Create(v, 0, sizeof(struct IDs));
		ASSERT_MEM_ABORT(*v);
	}
	id = XMLVector_Append(*v, NULL);
	ASSERT_MEM_ABORT(id);
	id->uri = NULL;
	id->id = strdup(idAtt->value);
	ASSERT_MEM_ABORT(id->id);
	a = XMLParser_GetNamedItem(parser, "uri");
	ASSERT_MEM_ABORT(a);
	return GetUriAndBase(a, &id->uri);
}

static int AddNextCatalog(LPXMLRUNTIMEATT cat)
{
	XMLCH **uri;
	ASSERT_MEM_ABORT(cat); /* shouldn't happen */
	if (!nextCatalog) {
		XMLVector_Create(&nextCatalog, 0, sizeof(char*));
		ASSERT_MEM_ABORT(nextCatalog);
	}
	uri = XMLVector_Append(nextCatalog, NULL);
	ASSERT_MEM_ABORT(uri);
	return GetUriAndBase(cat, uri);
}

static int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{
	if (!strcmp(qName, "public"))
		return AddUri(&publicIDS, XMLParser_GetNamedItem(parser, "publicId"));
	else if (!strcmp(qName, "system")) 
		return AddUri(&systemIDS, XMLParser_GetNamedItem(parser, "systemId"));
	else if (!strcmp(qName, "nextCatalog"))
		return AddNextCatalog(XMLParser_GetNamedItem(parser, "catalog"));
	return 0;
}

static void ErrorHandler(LPXMLPARSER parser)
{
	if (parser->ErrorCode != ERR_XMLP_ABORT) {	
		XMLCH *SystemID = XMLParser_GetSystemID(parser);
		XMLCH *curDoc = (SystemID) ? SystemID : doc;
		LPXMLDTDVALIDATOR v = (parser->ErrorCode==ERR_XMLP_VALIDATION) ?
			(LPXMLDTDVALIDATOR)parser->UserData : NULL;
		if (v) 
			fprintf(stderr, "%s:%d:%d: Validation error: %s\n", curDoc,
				 v->ErrorLine, v->ErrorColumn, v->ErrorString);
		else
			fprintf(stderr, "%s:%d:%d: Parsing error: %s\n", curDoc,
				 parser->ErrorLine, parser->ErrorColumn, parser->ErrorString);
	}
}

struct uriresolver_input_t *PrepNewInput(XMLCH *uri, XMLCH *base, int overridebase)
{
	curinput = Uriresolver_AddInput(curinput, uri);
	if (!curinput) goto EXITNOMEM;

	if (curinput->src) {
		if (uribuf.len) uribuf.len = 0;
		if (!(!base && overridebase)) {
			uri = (base) ? XMLStringbuf_Append(&uribuf, base, strlen(base)) :
				XMLStringbuf_Append(&uribuf, curinput->absolute->base, curinput->absolute->baselen);
			if (!uri) goto EXITNOMEM;
		}
		uri = XMLStringbuf_Append(&uribuf, curinput->src, strlen(curinput->src)+1);
		if (!uri) goto EXITNOMEM;
	}
	if (!(curinput->src = mfopen(uri, "rb"))) {
		fprintf(stderr, "Error opening file %s\n", uri);
		return NULL;
	}
	return curinput;
EXITNOMEM:
	fputs("Out of memory\n", stderr);
	return NULL;
}

static int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{	
	if (entity->publicID && !strcmp(entity->publicID,
		"-//OASIS//DTD Entity Resolution XML Catalog V1.0//EN")) 
		return XML_OK;

	reader->inputData = (entity->type == XML_ENTITY_DOCTYPE) ?
		PrepNewInput(entity->systemID, AppBase, 1) :
		PrepNewInput(entity->systemID, XMLParser_GetPrefixMapping(parser, "xml:base"), 0); 
	return (reader->inputData) ? XML_OK : XML_ABORT;
}

static int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{	
	struct uriresolver_input_t *inp = reader->inputData;
	if (ferror((FILE*)inp->src)) fputs("\nFile error\n", stderr);
	fclose(inp->src);
	curinput = Uriresolver_RemoveInput(inp);
	return XML_OK;
}

XMLCH *Catalogs_ResolveUri(XMLCH *publicID, XMLCH *systemID)
{	
	struct IDs *id = NULL;
	if (publicID && publicIDS && publicIDS->length)
		id = bsearch(publicID, publicIDS->array, publicIDS->length, 
			sizeof(struct IDs), idsearch);
	else if (systemID && systemIDS && systemIDS->length)
		id = bsearch(systemID, systemIDS->array, systemIDS->length, 
			sizeof(struct IDs), idsearch);
	return (id) ? id->uri : NULL;
}

LPXMLPARSER Catalogs_GetParser(void) { return parser; }

static void CleanupIds(LPXMLVECTOR v)
{	
	int i;
	for (i=0; i<v->length; i++) {
		struct IDs *id = XMLVector_Get(v, i);
		if (id->id) free(id->id);
		if (id->uri) free(id->uri);
	}
	XMLVector_Free(v);
}

void Catalogs_Cleanup(void) 
{	
	if (publicIDS) CleanupIds(publicIDS);
	if (systemIDS) CleanupIds(systemIDS);
	if (nextCatalog) {
		int i;
		char **uri;
		for (i=0; i<nextCatalog->length; i++) {
			uri = XMLVector_Get(nextCatalog, i);
			if (*uri) free(*uri);
		}
		XMLVector_Free(nextCatalog);
	}
}

static char *mstrtok(char **src, char *dst, size_t maxdst, int delim)
{
	char *s = *src;
	char *d = dst;
	while (*s && isspace(*s)) s++;
	while (1) {
		if (!*s || *s==delim) {
			if (d==dst) break;
			while(isspace(*(d-1))) d--;
			*src = (*s)?s+1:s;
			*d=0;
			return dst;
		}
		*d++ = *s++; 
		if ((size_t)(d - dst) >= maxdst ) 
			break; 
	}
	*src = (*s)?s+1:s;
	return NULL;
}

int Catalogs_Init(void)
{	
	LPXMLDTDVALIDATOR dtd;
	int ret=1, i=0;
	char t[FILENAME_MAX];
	char *env = getenv("XML_CATALOG_FILES");
	struct uriresolver_input_t startinp = { NULL, NULL, 0, NULL, &startinp };
	struct uriresolver_input_t *inp;

	curinput = &startinp;
	if (!env) doc = "/etc/xml/catalog";

	if (!XMLParser_Create(&parser)) {
			fputs("Out of memory\n", stderr);
			return 0;
	}
	if (!(dtd = XMLParser_CreateDTDValidator())) {
			XMLParser_Free(parser);
			fputs("Out of memory\n", stderr);
			return 0;
	}

	XMLParser_SetExternalSubset(parser, NULL, "catalog_flat.dtd");
	parser->startElementHandler = StartElement;
	parser->errorHandler = ErrorHandler;
	parser->resolveEntityHandler = ResolveEntity;
	parser->externalEntityParsedHandler = FreeInputData;
	
	while (ret) {
		if (env) doc = mstrtok(&env, t, sizeof(t)/sizeof(t[0]), ';');
		if (!doc) {
			if (env && *env) {
				ret = 0;
				fprintf(stderr, "Erroneous token in XML_CATALOG FILES: %s\n", env);
			}
			break;
		}

		while (doc) {
			if (!(inp = PrepNewInput(doc, NULL, 0))) {
				ret = 0;
				break;
			}			
			ret = XMLParser_ParseValidateDTD(dtd, parser, filestream, inp, 0);
			fclose(inp->src);
			curinput = Uriresolver_RemoveInput(inp);
			
			if (!ret || !nextCatalog || i == nextCatalog->length) 
				doc = NULL;
			else {
				char **uri = XMLVector_Get(nextCatalog, i++);
				doc = *uri;
			}
		}
	}
	
	if (ret) {
		if (publicIDS && publicIDS->length > 1) 
			qsort(publicIDS->array, publicIDS->length, sizeof(struct IDs), idsort);
		if (systemIDS && systemIDS->length > 1) 
			qsort(systemIDS->array, systemIDS->length, sizeof(struct IDs), idsort);
	}
	while (curinput != &startinp) {
		curinput = Uriresolver_RemoveInput(curinput);
	}
	XMLParser_FreeDTDValidator(dtd);
	XMLParser_Free(parser);
	return ret;
}
