#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"
#include "libparsifal/isrcmem.h"
#include "xmlplint.h"
#include "curlread.h"
#include "stimer.h"
#include "uriresolver.h"
#ifdef _MSC_VER
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif
#endif

int xmlSortAtts;
int xmlOutputFormat;
int xmlHasInternalSubset;
int xmlDoValidate;
int xmlIgnorableWS;
FILE *fout=NULL;
LPXMLPARSER parser;
char *xmlDTDUri;
static int xmlNsHandling;
static int xmlSkipExt;
static int xmlGenEnt;
static int xmlErrorRep;
static int xmlUseCatalogs;
static int xmlValidationWarn;
static int xmlShowStatus;
static int xmlFlags;
static size_t xmlInMemory;
static XMLCH *doc=NULL;
static char *xmlForeignDTD=NULL;
static CURLREADER *r=NULL;
char curlErrBuf2[CURL_ERROR_SIZE] = {'\0'};
char *AppBase;
XMLSTRINGBUF uribuf;
static struct uriresolver_input_t *curinput;
static int warnings;
static int gRet;
static int optind=1;
static char *optarg;

extern void rtrip_SetHandlers(LPXMLPARSER p);
extern void events_SetHandlers(LPXMLPARSER p);
extern int vfilter_Init(LPXMLDTDVALIDATOR dtd, char *uri);
extern void vfilter_Cleanup(LPXMLDTDVALIDATOR dtd);
extern int Catalogs_Init(void);
extern void Catalogs_Cleanup(void); 
extern XMLCH *Catalogs_ResolveUri(XMLCH *publicID, XMLCH *systemID);
extern void OutputEntity(const XMLCH *publicId, const XMLCH *systemId, 
						 const XMLCH *value, int type);

FILE *mfopen(const char *n, const char *m) 
{
	if (!strncmp(n, "file:", 5)) {
		for (n+=5; *n && *n=='/'; n++);
	}
	return (n && *n) ? fopen(n, m) : NULL;
}

static int mgetopt(int argc, char *argv[], char *opt)
{	
	char *p;
	optarg = NULL;
	
	if (optind >= argc) return -1;
	p = argv[optind];		
	if (!(*p == '-' && p[1]))
		return -1;
	
	optind++;
	opt = strchr(opt, p[1]);
	if (opt == NULL) return '?';

	if (opt[1] == ':') {
		if (p[2])
			optarg = p+2;
		else if (optind < argc) {
			if (argv[optind][0] == '-') {
				if (opt[2] != ':') return '?';
				return p[1];
			}
			optarg = argv[optind++];
		}
		else if (opt[2] != ':') 
			return '?';
	}
	else if (p[2]) return '?'; 
	return p[1];
}

static XMLCH *GetFormattedContext(LPXMLPARSER parser, int *col, int outputUTF8)
{
	int i, w, colb, cBytes;
	XMLCH *s, *r;

	if ((colb = XMLParser_GetContextBytes(parser, &s, &cBytes)) < 0) 
		return NULL;

	for (i=0, w=1; i!=cBytes; i++) {
		if (!s[i] || s[i]==0xD || s[i]==0xA) break;
		if (w && !isspace(s[i])) w = 0; /* all-whitespace flag */
	}
	
	if (i < colb || i > 255 || w) return NULL;
	
	if (outputUTF8) {
		if (parser->ErrorCode) {
			*col = (parser->ErrorCode == ERR_XMLP_VALIDATION) ? 
				((LPXMLDTDVALIDATOR)parser->UserData)->ErrorColumn :
				parser->ErrorColumn;	
		}
		else *col = XMLParser_GetCurrentColumn(parser);
		if (*col==-1) return NULL;
		(*col)--;
	}
	else *col = colb;

	/* alloc row len + size for \n, arrow and NUL: */
	if (!(r = malloc(i + *col + 3))) return NULL; 
	memcpy(r, s, i);
	
	for (w=0; w<colb; w++) { /* remove tabs */
		if (r[w]==0x9) r[w]=0x20;
	}
	
	r[i++]='\n';
	if (*col) {
		memset(r+i, '-', *col);
		i+=*col;
	}
	r[i]='^';
	r[++i]=0;
	(*col)++; /* column is always 1-based */
	return r;
}

#define WARNORERR ((xmlValidationWarn) ? "warning" : "error")

static void ErrorHandler(LPXMLPARSER parser) 
{	
	if (parser->ErrorCode == ERR_XMLP_ABORT) 
		gRet = RET_FATAL;
	else {
		XMLCH *SystemID = XMLParser_GetSystemID(parser);
		XMLCH *curDoc = (SystemID) ? SystemID : doc;
		LPXMLDTDVALIDATOR v = (parser->ErrorCode==ERR_XMLP_VALIDATION) ?
			(LPXMLDTDVALIDATOR)parser->UserData : NULL;

		if (xmlErrorRep == 1) {
			if (v) 
				fprintf(stderr, "%s:%d:%d: Validation %s: %s\n", curDoc,
					 v->ErrorLine, v->ErrorColumn, WARNORERR, v->ErrorString);
			else
				fprintf(stderr, "%s:%d:%d: Parsing error: %s\n", curDoc,
					 parser->ErrorLine, parser->ErrorColumn, parser->ErrorString);
		}
		else {
			int col;
			XMLCH *s;	
			LPXMLENTITY curEnt = XMLParser_GetCurrentEntity(parser);
			if (v)
				fprintf(stderr, "\nValidation %s: %s\nAt line %d col %d of resource %s\n", 
					WARNORERR, v->ErrorString, v->ErrorLine, v->ErrorColumn, curDoc);
			else
				fprintf(stderr, "\nParsing Error: %s\nAt line %d col %d of resource %s\n", 
					parser->ErrorString, parser->ErrorLine, parser->ErrorColumn, curDoc);
			if (curEnt && !curEnt->systemID) fprintf(stderr, "In entity: '%s'\n", curEnt->name);
			if ((s = GetFormattedContext(parser, &col, xmlErrorRep))) { 
			   fprintf(stderr, "%s\n", s);
			   free(s);
			}
		}
		if (xmlShowStatus) {
			if (v && xmlValidationWarn) {
				warnings++;
				return;
			}
			xmlShowStatus = -1;
		}
		
		if (v) {
			if (v->ErrorCode==ERR_XMLDTDV_MEMORY_ALLOC) gRet = RET_FATAL;
			else if (gRet==RET_SUCCESS) gRet = RET_NOTVALID;
		}
		else gRet = (parser->ErrorCode==ERR_XMLP_MEMORY_ALLOC) ? RET_FATAL : RET_NOTWF;
	}
}

static CURLREADER *NewCurlreader(char *doc, char *errbuf)
{
	CURLREADER *r =  Curlreader_Create();
	if (!r)
		fprintf(stderr, "Fatal error creating Curlreader!\n");
	else {
		curl_easy_setopt(r->curlh, CURLOPT_ERRORBUFFER, errbuf);
		curl_easy_setopt(r->curlh, CURLOPT_FOLLOWLOCATION, 1);
		Curlreader_Open(r, doc);
	}
	return r;
}

int curlstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = Curlreader_Read(((struct uriresolver_input_t*)inputData)->src, buf, 1, cBytes);
	return (*cBytesActual < cBytes);
}

int filestream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = fread(buf, 1, cBytes, ((struct uriresolver_input_t*)inputData)->src);
	return (*cBytesActual < cBytes);
}

int MemInputsrc(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	/* note that we cant use XMLMEMINPUTSRC_HANDLE 'cos inputData is uriresolver */
	LPXMLMEMINPUTSRC inp = ((struct uriresolver_input_t*)inputData)->src;
	if ((inp->cBytes + cBytes) < inp->cTotal) {
        memcpy(buf, inp->pBuf + inp->cBytes, cBytes);
        *cBytesActual = cBytes;
        inp->cBytes += cBytes;
        return 0;
    }
    else {
        *cBytesActual = inp->cTotal - inp->cBytes;
        if (*cBytesActual) {
            memcpy(buf, inp->pBuf + inp->cBytes, *cBytesActual);
            inp->cBytes += *cBytesActual; 
		}
        return 1;
    }
}

static int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{
	XMLCH *systemID = entity->systemID;
	XMLCH *uri;
	
	if (xmlUseCatalogs) {
		if (xmlUseCatalogs == 1) {
			if (!Catalogs_Init()) return XML_ABORT;
			xmlUseCatalogs = -1;
		}
		uri = Catalogs_ResolveUri(entity->publicID, entity->systemID);
		if (uri) systemID = uri;
	}
	
	curinput = Uriresolver_AddInput(curinput, systemID);
	ASSERT_MEM_ABORT(curinput);
	
 	if (!curinput->src) { /* absolute or no base exists at all */
		if (uri != systemID) uri = systemID;
	}
	else {
		XMLCH *s = (entity->type == XML_ENTITY_DOCTYPE) ? NULL : 
			XMLParser_GetPrefixMapping(parser, "xml:base");
		if (uribuf.len) uribuf.len = 0;
		uri = (s) ? XMLStringbuf_Append(&uribuf, s, strlen(s)) :
			XMLStringbuf_Append(&uribuf, curinput->absolute->base, 
				curinput->absolute->baselen);		
		ASSERT_MEM_ABORT(uri);
		uri = XMLStringbuf_Append(&uribuf, curinput->src, strlen(curinput->src)+1);
		ASSERT_MEM_ABORT(uri);
	}

	if (entity->type == XML_ENTITY_DOCTYPE &&
		xmlOutputFormat == XMLOUTPUTFORMAT_ROUNDRIP) {
		/* turn off DTD declaration handlers for external DTDs
		   when roundtripping: */			
		if (xmlDoValidate)
			((LPXMLDTDVALIDATOR)UserData)->elementDeclHandler = NULL;
		else
			parser->elementDeclHandler = NULL;
		parser->attributeDeclHandler = NULL;
		parser->entityDeclHandler = NULL;
		parser->notationDeclHandler = NULL;
	}
	else if (xmlOutputFormat == XMLOUTPUTFORMAT_EVENTS) {
		fprintf(PFOUT, "resolveEntity(name {%s} ", entity->name);
		OutputEntity(entity->publicID, systemID, NULL, entity->type);
	}

	curinput->src = mfopen(uri, "rb");
	if (curinput->src)
		reader->inputsrc = filestream;
	else {	
		curinput->src = NewCurlreader(uri, curlErrBuf2);
		if (!curinput->src) return XML_ABORT;
		reader->inputsrc = curlstream;
	}
	reader->inputData = curinput;
	return 0;
}

static int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{	
	struct uriresolver_input_t *inp = reader->inputData;

	if (reader->inputsrc == filestream) {
		if (ferror((FILE*)inp->src)) fputs("\nFile error\n", stderr);
		fclose((FILE*)inp->src);
	}
	else {
		if (Curlreader_ReadStatusCode(inp->src) != CURLE_OK) {
			fprintf(stderr, "\nLibcurl Error: %s\n", 
				(*curlErrBuf2) ? curlErrBuf2 : "unknown/bad url");
		}
		Curlreader_Free(inp->src);
	}
	
	curinput = Uriresolver_RemoveInput(inp);

	if (xmlOutputFormat == XMLOUTPUTFORMAT_EVENTS)
		fprintf(PFOUT, "externalEntityParsed(name {%s})\n", entity->name);
	return 0;
}

static void usage(void)
{
	fputs(
	"Usage: xmlplint [options] <file/url>\n"
	"Options:\n"
	"  -V Validate document\n"
	"  -a Sort attributes\n"
	"  -v Show version info\n"
	"  -x Skip external entities\n"
	"  -M Load document into memory before processing\n"
	"     (useful for benchmarking)\n"
	"  -i Report ignorable whitespace\n"
	"  -c Use catalogs file /etc/xml/catalog or files set by\n"
	"     XML_CATALOG_FILES environment variable (semicolon separated)\n"
	"  -W Treat validation errors as warnings\n"
	"  -s Show error/warning status\n"
	"  -d Load foreign DTD e.g. -d /home/mydtd.dtd\n"
	"  -o Output to file\n"
	"  -f Output format (default: roundtrip)\n"
	"     1: SAX events\n"
	"     2: canonical form (http://www.jclark.com/xml/canonxml.html)\n"
	"     3: silent - errors only\n"
	"  -e Force input encoding e.g. -e ISO-8859-1\n"
	"  -X General entity settings\n"
	"     1: Preserve general entities\n"
	"     2: Treat undefined entities as errors\n"
	"  -n Namespace handling (default: on)\n"
	"     1: off\n"
	"     2: report declarations\n"
	"  -u Base uri\n"
	"  -U Uri for DTD validation filter\n"
	"  -t Benchmark processing time for specified iterations\n"
	"     e.g. -t 1000\n"
	"  -E Error reporting (default: verbose)\n"
	"     1: simple (useful for editors)\n"
	"     2: output displays UTF-8 (for column info)\n"
	"  -F Set spesific parser flag(s) (see parsifal.h)\n"
	"Example:\n"
	"  xmlplint -V -f1 xmlfile.xml -o outfile.xml\n\n"
	,stderr);
	exit(RET_SUCCESS);
}

int main(int argc, char* argv[])
{	
	LPXMLDTDVALIDATOR dtd = NULL;
	LPFNINPUTSRC stream = filestream;
	XMLMEMINPUTSRC meminput;
	STIMER timer;
	int iter = 0;
	int n = 0;
	int timed;
	int ret = RET_SUCCESS;
	char *encoding = NULL;
	char *s;
	void *input; /* most likely same as curinput->src but for cleaning up after parse etc. */
	char curlErrBuf[CURL_ERROR_SIZE] = {'\0'};
	struct uriresolver_input_t startinp = { NULL, NULL, 0, NULL, &startinp };

	#ifdef _MSC_VER
	#ifdef _DEBUG
		int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
		tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag( tmpFlag );
	#endif
	#endif
	
	curinput = &startinp;
	XMLStringbuf_Init(&uribuf, 256, 0);

	s = strrchr(argv[0], '/');	
#ifdef _WIN32
	if (!s) s = strrchr(argv[0], '\\');
#endif
	if (!s) AppBase = NULL;
	else {
		if (!(AppBase = strdup(argv[0]))) {
			fputs("Out of memory!\n", stderr);
			return RET_FATAL;
		}
		AppBase[(s-argv[0])+1] = '\0';
	}

	while (optind < argc) { /* to allow argument (case -1) in any place */
		int c = mgetopt(argc, argv, "VavxMicWsd:o:f:e:n:u:U:t:E:X:F:");
		switch(c) {
			case 'V': xmlDoValidate = 1; break;
			case 'a': xmlSortAtts = 1; break;
			case 'v':
				fprintf(stderr, "xmlplint %s\n- Parsifal XML Parser %s\n- %s\n", 
					VER_XMLPLINT, XMLParser_GetVersionString(), curl_version());
				return RET_SUCCESS;
			case 'x': xmlSkipExt = 1; break;
			case 'M': xmlInMemory = 1; break;
			case 'i': xmlIgnorableWS = 1; break;
			case 'c': xmlUseCatalogs = 1; break;
			case 'W': xmlValidationWarn = 1; break;
			case 's': xmlShowStatus = 1; break;
			case 'd': xmlForeignDTD = optarg; break;
			case 'o':
				if (fout) usage();				
				if (!(fout = mfopen(optarg, "wb"))) {
					fprintf(stderr, "Error opening file %s\n", optarg);
					return RET_IOERR;
				}
				break;
			case 'f':
				xmlOutputFormat = atoi(optarg);
				if (xmlOutputFormat < 1 || xmlOutputFormat > 3) usage();
				break;
			case 'e': encoding = optarg; break;	
			case 'n':
				xmlNsHandling = atoi(optarg);
				if (xmlNsHandling < 1 || xmlNsHandling > 2) usage();
				break;
			case 'u': 
				curinput->base = optarg;
				curinput->baselen = strlen(optarg);
				break;
			case 'U': xmlDTDUri = optarg; break;
			case 't':
				iter = atoi(optarg);
				if (iter < 1) usage();
				break;
			case 'E':
				xmlErrorRep = atoi(optarg);
				if (xmlErrorRep < 1 || xmlErrorRep > 2) usage();
				break;
			case 'X':
				xmlGenEnt = atoi(optarg);
				if (xmlGenEnt < 1 || xmlGenEnt > 2) usage();
				break;
			case 'F':
				xmlFlags = atoi(optarg);
				if (xmlFlags < 1) usage();
				break;
			case -1:
				if (!doc) {
					doc = argv[optind++];
					break;
				}			
			default: usage();
		}
	}

	if (!doc) {
		input = curinput->src = stdin;
		doc="<stdin>";
	}
	else {
		input = mfopen(doc, "rb");
		if (!input) {
			r = NewCurlreader(doc, curlErrBuf);
			if (r==NULL) return RET_FATAL;
			input=r;
			stream=curlstream;
		}
		if (xmlInMemory) {
			char *memfile;
			if (r) {
				fputs("In memory (-M) document must be local file!\n", stderr);
				return RET_FATAL;
			}
			fseek(input, 0, SEEK_END);
			xmlInMemory = ftell(input);
			if (!xmlInMemory || !(memfile = malloc(xmlInMemory))) {
				fprintf(stderr, "Error allocating memfile (%d bytes)\n", xmlInMemory);
				return RET_FATAL;
			}
			fseek(input, 0, SEEK_SET);
			if (fread(memfile, 1, xmlInMemory, input) != xmlInMemory) {
				fprintf(stderr, "fread error reading memfile (%d bytes)\n", xmlInMemory);
				return RET_FATAL;
			}
			XMLMEMINPUTSRC_INIT(&meminput, memfile, xmlInMemory);
			stream=MemInputsrc;
			fclose(input);
			input=&meminput;
		}
		if (!xmlSkipExt) {
			curinput = Uriresolver_AddInput(curinput, doc);
			if (!curinput) {
				fputs("Out of memory!\n", stderr);
				return RET_FATAL;
			}
		}
		curinput->src = input;
	}

	if (!XMLParser_Create(&parser)) {
		fputs("Error creating parser!\n", stderr);
		return RET_FATAL;
	}

	if (curl_global_init(CURL_GLOBAL_ALL)) {
		fputs("Error Initializing libcurl!\n", stderr);
		return RET_FATAL;
	}

	if (xmlNsHandling == 1) 
		_XMLParser_SetFlag(parser, XMLFLAG_NAMESPACES, 0);
	else if (xmlNsHandling == 2)
		_XMLParser_SetFlag(parser, XMLFLAG_NAMESPACE_PREFIXES, 1);
	
	if (xmlSkipExt) /* we'll skip external [dtd] too */
		_XMLParser_SetFlag(parser, XMLFLAG_EXTERNAL_GENERAL_ENTITIES, 0);
	else {
		parser->resolveEntityHandler = ResolveEntity;
		parser->externalEntityParsedHandler = FreeInputData;
	}
	
	if (xmlGenEnt == 1)
		_XMLParser_SetFlag(parser, XMLFLAG_PRESERVE_GENERAL_ENTITIES, 1);
	else if (xmlGenEnt == 2)
		_XMLParser_SetFlag(parser, XMLFLAG_UNDEF_GENERAL_ENTITIES, 1);

	if (xmlForeignDTD)
		XMLParser_SetExternalSubset(parser, NULL, xmlForeignDTD);

	if (xmlFlags) parser->XMLFlags |= xmlFlags;

	parser->errorHandler = ErrorHandler;
	
	if (xmlValidationWarn) {
		_XMLParser_SetFlag(parser, XMLFLAG_VALIDATION_WARNINGS, 1);
		if (!xmlDoValidate) xmlDoValidate = 1;
	}

	if (!fout && xmlOutputFormat != XMLOUTPUTFORMAT_SILENT) 
		fout = stdout;
	
	if (xmlOutputFormat == XMLOUTPUTFORMAT_CANONICAL ||
		xmlOutputFormat == XMLOUTPUTFORMAT_ROUNDRIP)
		rtrip_SetHandlers(parser);
	else if (xmlOutputFormat == XMLOUTPUTFORMAT_EVENTS)
		events_SetHandlers(parser);
	
	if (iter) {
		if (!xmlInMemory) {
			fputs("Timing works only with in memory (-M) document\n", stderr);
			return RET_FATAL;
		}
		timed = 1;
		STimer_Start(&timer);
	}
	else {
		timed = 0;
		iter = 1;
	}
	
	if (xmlDoValidate) {	
		if ((dtd = XMLParser_CreateDTDValidator())) {
			if (xmlDTDUri && !vfilter_Init(dtd, xmlDTDUri)) return RET_FATAL;
			while(1) {
				XMLParser_ParseValidateDTD(dtd, parser, stream, curinput, encoding);
				if (++n == iter) break;
				meminput.cBytes=0;
			}
			if (xmlDTDUri) vfilter_Cleanup(dtd);
			XMLParser_FreeDTDValidator(dtd);
		}
		else {
			fprintf(stderr, "Error creating validator!\n");
			ret = RET_FATAL;
		}
	}
	else {
		while(1) {
			XMLParser_Parse(parser, stream, curinput, encoding);
			if (++n == iter) break;
			meminput.cBytes=0;
		}
	}

	if (timed) {
		STimer_Stop(&timer);
		fprintf(stderr, "\nDocument(s) parsed in %ld ms\n", timer.msResult);
		if (iter > 1) fprintf(stdout, "%d iterations. Average: %ld ms\n", iter, timer.msResult/iter);
	}
	
	if (stream==curlstream) {	
		if (Curlreader_ReadStatusCode(input) != CURLE_OK) {
			fprintf(stderr, "\nLibcurl Error: %s\n", 
				(*curlErrBuf) ? curlErrBuf : "unknown/bad url");
			ret = RET_IOERR;
		}
		Curlreader_Free(r);
	}
	else if (xmlInMemory)
		free(meminput.pBuf);
	else {
		if (ferror((FILE*)input)) { 
			fprintf(stderr, "\n%s file error\n", doc);
			ret = RET_IOERR;
		}
		if (input != stdin) fclose(input);
	}
	
	XMLStringbuf_Free(&uribuf);
	while (curinput != &startinp) {
		curinput = Uriresolver_RemoveInput(curinput);
	}
	if (AppBase) free(AppBase);
	if (xmlUseCatalogs) Catalogs_Cleanup();
	if (fout && fout != stdout) fclose(fout);
	XMLParser_Free(parser);
	curl_global_cleanup();
	if (xmlShowStatus)
		fprintf(stderr, "xmlplint - %d warning(s), %d error(s)\n",
			warnings, (xmlShowStatus==1) ? 0 : 1);	
	
	if (ret != RET_SUCCESS) return ret;
	return (gRet) ? gRet : RET_SUCCESS;
}

