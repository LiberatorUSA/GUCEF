#include <stdlib.h>
#include <string.h>
#include "uriresolver.h"

struct uriresolver_input_t *Uriresolver_RemoveInput(struct uriresolver_input_t *cur) 
{	
	struct uriresolver_input_t *r = cur->prev;
	if (cur->base) free(cur->base);
	free(cur);
	return r;
}

struct uriresolver_input_t *Uriresolver_AddInput(struct uriresolver_input_t *cur, XMLCH *uri)
{	
	struct uriresolver_input_t *inp = malloc(sizeof(*inp));
	XMLCH *r = strrchr(uri, '/');
#ifdef _WIN32
	if (!r) r = strrchr(uri, '\\');
#endif
	if (!inp) return NULL;
	inp->prev = cur;
	
	if (r && r[1]) { 
		size_t len = (r-uri)+1;
		if (!cur->absolute->base || *uri == *r || strchr(uri, ':')) { 
			/* no base exists or absolute: */
			inp->base = malloc(len+1);
			if (!inp->base) return NULL;
			memcpy(inp->base, uri, len);
			inp->src = NULL;
		}
		else { /* relative, have to concat base to this
			uri and 'return' filename in inp->src: */
			struct uriresolver_input_t *ca = cur->absolute;
			inp->base = malloc(ca->baselen+len+1);
			if (!inp->base) return NULL;
			memcpy(inp->base, ca->base, ca->baselen);
			memcpy(inp->base+ca->baselen, uri, len);
			inp->src = uri+len;
			len += ca->baselen;
		}
		inp->base[len] = '\0';
		inp->baselen = len;
		inp->absolute = inp; /* this becomes current absolute uri */
	}
	else { /* no path */
		inp->absolute = cur->absolute;
		inp->base = NULL;
		inp->src = (cur->absolute->base) ? uri : NULL; 
		/* if base exists set inp->src == uri */ 
	}
	return inp;
}
