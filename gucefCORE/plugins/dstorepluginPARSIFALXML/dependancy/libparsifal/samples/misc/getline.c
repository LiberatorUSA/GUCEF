#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define BAS3FROMM3MB3R(basetype, pmember, member) (basetype*)((char*)pmember - offsetof(basetype, member))

struct GetLine_t {
	FILE *f;
	char *buf;
	size_t bufsize;
	size_t lineLen;
};

FILE **GetLine_Fopen(const char *filename, const char *mode)
{
	struct GetLine_t *g = malloc(sizeof *g);
	if (!g) return NULL;
	if (!filename) 
		g->f = stdin;
	else {
		g->f = fopen(filename, mode);
		if (!g->f) return NULL;
	}
	g->bufsize=256;
	g->buf = malloc(g->bufsize);
	if (!g->buf) return NULL;
	return &g->f;
}

int GetLine_Fclose(FILE **f)
{
	struct GetLine_t *g = BAS3FROMM3MB3R(struct GetLine_t, f, f);
	int r = (g->f == stdin) ? 0 : fclose(*f);
	free(g->buf);
	free(g);
	return r;
}

char *GetLine_Read(FILE **f)
{	 
	int ch;
	struct GetLine_t *g = BAS3FROMM3MB3R(struct GetLine_t, f, f);
	g->lineLen=0;

	while(g->buf) {
		for(; g->lineLen<g->bufsize; g->buf[g->lineLen++]=ch) {
			ch = fgetc(g->f);
			if (ch == EOF || ch == '\n') {
				if (!g->lineLen && ch == EOF) return NULL;
				g->buf[g->lineLen] = '\0';
				return g->buf;
			}
		}
		g->bufsize += g->bufsize;
		g->buf = realloc(g->buf, g->bufsize);
	}
	return NULL;
}

