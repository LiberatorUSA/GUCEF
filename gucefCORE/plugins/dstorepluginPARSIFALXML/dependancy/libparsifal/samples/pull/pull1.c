#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xmlreader.h"

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = fread(buf, 1, cBytes, (FILE*)inputData);	
	return (*cBytesActual < cBytes);
}

int main(int argc, char* argv[])
{	
	struct XMLReader_t r;

	if (!XMLReader_Create(&r, 0)) {
		puts("Error creating parser");
		return EXIT_FAILURE;
	}

	if (XMLReader_Parse(&r, cstream, stdin, 0)) {
		union XMLReaderNode_t *rn;
		int i;

		while((rn = XMLReader_Read(&r))) {
			if (rn->type == XMLREADERTYPE_STARTELEMENT) {
				if (!strcmp((const char*)rn->startelement.qname, "toni.uusitalo")) {
					printf("Don't be silly, illegal naming policy of xml element,\n"
						"Your parsing will be terminated immediately!\n");
					XMLReader_AbortParse(&r);
					break; /* note: DON'T call Read after AbortParse */
				}
				printf("STARTELEMENT -- uri: %s localName: %s qName: %s\n"
					"  attributes count: %d\n", 
						rn->startelement.uri,
						rn->startelement.localname,
						rn->startelement.qname,
						rn->startelement.atts->length);
				for(i=0; i<rn->startelement.atts->length; i++) {
					struct XMLReaderAttribute_t *a = XMLVector_Get(rn->startelement.atts, i);
					printf("  name: %s value: %s\n", 
						XMLReader_GetAttributeQName(&r, a),  
						XMLReader_GetAttributeValue(&r, a));
				}
			}
			else if	(rn->type == XMLREADERTYPE_ENDELEMENT) {
				printf("ENDELEMENT -- uri: %s localName: %s qName: %s\n",
						rn->endelement.uri,
						rn->endelement.localname,
						rn->endelement.qname);
			}
			else if	(rn->type == XMLREADERTYPE_CHARS) {
				printf("CHARS -- text: %s length: %d\n",
						rn->chars.text, rn->chars.length);
			}
			else if	(rn->type == XMLREADERTYPE_COMMENT) {
				printf("COMMENT -- text: %s length: %d\n",
						rn->comment.text, rn->comment.length);
			}
			else if	(rn->type == XMLREADERTYPE_PI) {
				printf("PROCESSINGINSTRUCTION -- target: %s data: %s\n",
						rn->pi.target, rn->pi.data);
			}	
		} /* while read != NULL */
	} /* parse ok */

	if (r.errorcode) {
		if (r.errorcode == ERR_XMLP_VALIDATION)
			fprintf(stderr, "\nValidation Error: %s\nAt line %d, col %d, code %d\n", 
				r.dtdv->ErrorString, r.dtdv->ErrorLine, r.dtdv->ErrorColumn, r.dtdv->ErrorCode);
		else
			fprintf(stderr, "\nParsing Error: %s\nAt line %d, col %d, code %d\n", 
				r.parser->ErrorString, r.parser->ErrorLine, r.parser->ErrorColumn, r.errorcode);
	
	}
	
	XMLReader_Free(&r);
	return 0;
}
