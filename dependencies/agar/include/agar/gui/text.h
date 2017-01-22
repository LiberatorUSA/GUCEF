/*	Public domain	*/

#ifndef _AGAR_GUI_TEXT_H_
#define _AGAR_GUI_TEXT_H_

#include <agar/gui/surface.h>
#include <agar/gui/drv.h>

#include <agar/gui/begin.h>

#define AG_GLYPH_NBUCKETS 1024	/* Buckets for glyph cache table */
#define AG_TEXT_STATES_MAX 128	/* Maximum number of saved text states */

struct ag_window;
struct ag_button;

enum ag_text_justify {
	AG_TEXT_LEFT,
	AG_TEXT_CENTER,
	AG_TEXT_RIGHT
};
enum ag_text_valign {
	AG_TEXT_TOP,
	AG_TEXT_MIDDLE,
	AG_TEXT_BOTTOM
};
enum ag_text_msg_title {
	AG_MSG_ERROR,
	AG_MSG_WARNING,
	AG_MSG_INFO
};

struct ag_font;

/* Font specification (may be obtained from fontconfig) */
enum ag_font_type {
	AG_FONT_VECTOR,				/* Vectorial font */
	AG_FONT_BITMAP				/* Raw glyph pixmaps */
};
enum ag_font_spec_source {
	AG_FONT_SOURCE_FILE,			/* Load from file */
	AG_FONT_SOURCE_MEMORY			/* Read from memory */
};
typedef struct ag_font_spec {
	enum ag_font_type type;
	enum ag_font_spec_source sourceType;
	union {
		char file[AG_PATHNAME_MAX];	/* Font file */
		struct {
			const Uint8 *data;	/* Memory region */
			size_t size;
		} mem;
	} source;
	int index;				/* Font index */
	double size;				/* Font size */
	struct {				/* Transform matrix */
		double xx, xy;
		double yx, yy;
	} matrix;
} AG_FontSpec;

/* Cached glyph surface/texture information. */
typedef struct ag_glyph {
	struct ag_font *font;		/* Font face */
	AG_Color        color;		/* Glyph color */
	Uint32          ch;		/* Unicode character */
	AG_Surface     *su;		/* Rendered surface */
	int             advance;	/* Pixel advance */
	Uint            texture;	/* Cached texture (driver-specific) */
	AG_TexCoord     texcoords;	/* Texture coordinates */
	AG_SLIST_ENTRY(ag_glyph) glyphs;
} AG_Glyph;

/* Loaded font */
typedef struct ag_font {
	struct ag_object obj;
	AG_FontSpec spec;		/* Input font specification */
	Uint flags;
#define AG_FONT_BOLD		0x01	/* Render as bold */
#define AG_FONT_ITALIC		0x02	/* Render as italic */
#define AG_FONT_UNDERLINE	0x04	/* Render with underline */
#define AG_FONT_UPPERCASE	0x08	/* Force uppercase display */
	int height;			/* Body size in pixels */
	int ascent;			/* Ascent (relative to baseline) */
	int descent;			/* Descent (relative to baseline) */
	int lineskip;			/* Multiline y-increment */
	void *ttf;			/* TTF object */
	char bspec[32];			/* Bitmap font specification */
	AG_Surface **bglyphs;		/* Bitmap glyphs */
	Uint nglyphs;			/* Bitmap glyph count */
	Uint32 c0, c1;			/* Bitmap glyph range */
	Uint nRefs;			/* Reference count */
	AG_TAILQ_ENTRY(ag_font) fonts;
} AG_Font;

/*
 * State variables for text rendering.
 * SYNC: AG_TextStateCompare()
 */
typedef struct ag_text_state {
	AG_Font *font;			/* Font face */
	AG_Color color;			/* Foreground text color */
	AG_Color colorBG;		/* Background color */
	enum ag_text_justify justify;	/* Justification mode */
	enum ag_text_valign valign;	/* Vertical alignment mode */
	int tabWd;			/* Width of \t in pixels */
} AG_TextState;

/* Description of font stored in data segment. */
typedef struct ag_static_font {
	const char *name;		/* Identifier */
	enum ag_font_type type;		/* Type of font */
	Uint32 size;			/* Size in bytes */
	const Uint8 *data;		/* Font data */
	AG_Font *font;			/* Initialized font structure */
} AG_StaticFont;

/* Measures of rendered text. */
typedef struct ag_text_metrics {
	int w, h;			/* Dimensions in pixels */
	Uint *wLines;			/* Width of each line */
	Uint  nLines;			/* Total line count */
} AG_TextMetrics;

typedef struct ag_glyph_cache {
	AG_SLIST_HEAD_(ag_glyph) glyphs;
} AG_GlyphCache;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_ObjectClass agFontClass;
extern DECLSPEC AG_Font *agDefaultFont;
extern DECLSPEC int agTextFontHeight;
extern DECLSPEC int agTextFontAscent;
extern DECLSPEC int agTextFontDescent;
extern DECLSPEC int agTextFontLineSkip;
extern DECLSPEC int agFreetypeInited;
extern DECLSPEC int agRTL;
extern DECLSPEC int agGlyphGC;
extern DECLSPEC AG_TextState *agTextState;
extern DECLSPEC AG_Mutex agTextLock;
extern DECLSPEC AG_StaticFont *agBuiltinFonts[];
extern DECLSPEC const int agBuiltinFontCount;
extern DECLSPEC int AG_InitTextSubsystem(void);
extern DECLSPEC void AG_DestroyTextSubsystem(void);
extern DECLSPEC void AG_TextParseFontSpec(const char *);
extern DECLSPEC AG_Font *AG_FetchFont(const char *, int, int);
extern DECLSPEC void AG_UnusedFont(AG_Font *);
extern DECLSPEC void AG_SetDefaultFont(AG_Font *);
extern DECLSPEC void AG_SetRTL(int);
extern DECLSPEC void AG_PushTextState(void);
extern DECLSPEC void AG_PopTextState(void);
extern DECLSPEC AG_Font *AG_TextFontLookup(const char *, int, Uint);
extern DECLSPEC AG_Font *AG_TextFontPts(int);
extern DECLSPEC AG_Font *AG_TextFontPct(int);
extern DECLSPEC void AG_TextSize(const char *, int *, int *);
extern DECLSPEC void AG_TextSizeMulti(const char *, int *, int *, Uint **, Uint *);
extern DECLSPEC void AG_TextSizeUCS4(const Uint32 *, int *, int *);
extern DECLSPEC void AG_TextSizeMultiUCS4(const Uint32 *, int *, int *, Uint **, Uint *);
extern DECLSPEC AG_Surface *AG_TextRenderf(const char *, ...);
extern DECLSPEC AG_Surface *AG_TextRenderUCS4(const Uint32 *);
extern DECLSPEC void AG_TextMsgS(enum ag_text_msg_title, const char *);
extern DECLSPEC void AG_TextMsg(enum ag_text_msg_title, const char *, ...) FORMAT_ATTRIBUTE(printf,2,3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_TextTmsgS(enum ag_text_msg_title, Uint32, const char *);
extern DECLSPEC void AG_TextTmsg(enum ag_text_msg_title, Uint32, const char *, ...) FORMAT_ATTRIBUTE(printf,3,4) NONNULL_ATTRIBUTE(3);
extern DECLSPEC void AG_TextInfoS(const char *, const char *);
extern DECLSPEC void AG_TextInfo(const char *, const char *, ...) FORMAT_ATTRIBUTE(printf,2,3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_TextWarningS(const char *, const char *);
extern DECLSPEC void AG_TextWarning(const char *, const char *, ...) FORMAT_ATTRIBUTE(printf,2,3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_TextErrorS(const char *);
extern DECLSPEC void AG_TextError(const char *, ...) FORMAT_ATTRIBUTE(printf,1,2) NONNULL_ATTRIBUTE(1);
extern DECLSPEC void AG_TextEditFloat(double *, double, double, const char *, const char *, ...) FORMAT_ATTRIBUTE(printf,5,6) NONNULL_ATTRIBUTE(5);
extern DECLSPEC void AG_TextEditString(char *, size_t, const char *, ...) FORMAT_ATTRIBUTE(printf,3,4) NONNULL_ATTRIBUTE(3);
extern DECLSPEC struct ag_window *AG_TextPromptOptions(struct ag_button **, Uint, const char *, ...);
extern DECLSPEC void AG_TextInitGlyphCache(AG_Driver *);
extern DECLSPEC void AG_TextClearGlyphCache(AG_Driver *);
extern DECLSPEC void AG_TextDestroyGlyphCache(AG_Driver *);
extern DECLSPEC AG_Glyph *AG_TextRenderGlyphMiss(AG_Driver *, Uint32);
extern DECLSPEC void AG_TextAlign(int *, int *, int, int, int, int, int, int, int, int, enum ag_text_justify, enum ag_text_valign);
#define AG_TextMsgFromError() AG_TextMsgS(AG_MSG_ERROR, AG_GetError())

static __inline__ int
AG_TextStateCompare(const AG_TextState *s1, const AG_TextState *s2)
{
	if (s1->font == s2->font &&
	    AG_ColorCompare(s1->color,s2->color) == 0 &&
	    AG_ColorCompare(s1->colorBG,s2->colorBG) == 0 &&
	    s1->justify == s2->justify &&
	    s1->valign == s2->valign &&
	    s1->tabWd == s2->tabWd) {
		return (0);
	}
	return (1);
}
/*
 * Return the offset in pixels needed to align text based on the current
 * justification mode.
 */

static __inline__ int
AG_TextJustifyOffset(int w, int wLine)
{
	switch (agTextState->justify) {
	case AG_TEXT_LEFT:	return (0);
	case AG_TEXT_CENTER:	return (w/2 - wLine/2);
	case AG_TEXT_RIGHT:	return (w - wLine);
	}
	return (0);
}
/*
 * Return the offset in pixels needed to align text based on the current
 * vertical alignment mode.
 */

static __inline__ int
AG_TextValignOffset(int h, int hLine)
{
	switch (agTextState->valign) {
	case AG_TEXT_TOP:	return (0);
	case AG_TEXT_MIDDLE:	return (h/2 - hLine/2);
	case AG_TEXT_BOTTOM:	return (h - hLine);
	}
	return (0);
}
/*
 * Allocate a transparent surface and render text from a C string.
 * The string may contain UTF-8 sequences.
 */

static __inline__ AG_Surface *
AG_TextRender(const char *text)
{
	Uint32 *ucs;
	AG_Surface *su;
	
	if ((ucs = AG_ImportUnicode("UTF-8", text, NULL, NULL)) != NULL) {
		su = AG_TextRenderUCS4(ucs);
		AG_Free(ucs);
		return (su);
	}
	return (NULL);
}
/*
 * Lookup/insert a glyph in the glyph cache.
 * Must be called from GUI rendering context.
 */

static __inline__ AG_Glyph *
AG_TextRenderGlyph(AG_Driver *drv, Uint32 ch)
{
	AG_Glyph *gl;
	Uint h = (Uint)(ch % AG_GLYPH_NBUCKETS);

	AG_SLIST_FOREACH(gl, &drv->glyphCache[h].glyphs, glyphs) {
		if (ch == gl->ch &&
		    agTextState->font == gl->font &&
		    AG_ColorCompare(agTextState->color,gl->color) == 0)
			break;
	}
	if (gl == NULL) {
		gl = AG_TextRenderGlyphMiss(drv, ch);
		AG_SLIST_INSERT_HEAD(&drv->glyphCache[h].glyphs, gl, glyphs);
	}
	return (gl);
}

static __inline__ void
AG_TextColor(AG_Color C)
{
	AG_MutexLock(&agTextLock);
	agTextState->color = C;
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextColorRGB(Uint8 r, Uint8 g, Uint8 b)
{
	AG_MutexLock(&agTextLock);
	agTextState->color = AG_ColorRGB(r,g,b);
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextColorRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	AG_MutexLock(&agTextLock);
	agTextState->color = AG_ColorRGBA(r,g,b,a);
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextColorHex(Uint32 c)
{
	AG_MutexLock(&agTextLock);
	agTextState->color = AG_ColorHex(c);
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextBGColor(AG_Color C)
{
	AG_MutexLock(&agTextLock);
	agTextState->colorBG = C;
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextBGColorRGB(Uint8 r, Uint8 g, Uint8 b)
{
	AG_MutexLock(&agTextLock);
	agTextState->colorBG = AG_ColorRGB(r,g,b);
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextBGColorRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	AG_MutexLock(&agTextLock);
	agTextState->colorBG = AG_ColorRGBA(r,g,b,a);
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextBGColorHex(Uint32 c)
{
	AG_MutexLock(&agTextLock);
	agTextState->colorBG = AG_ColorHex(c);
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextFont(AG_Font *font)
{
	AG_MutexLock(&agTextLock);
	agTextState->font = font;
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextJustify(enum ag_text_justify mode)
{
	AG_MutexLock(&agTextLock);
	agTextState->justify = mode;
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextValign(enum ag_text_valign mode)
{
	AG_MutexLock(&agTextLock);
	agTextState->valign = mode;
	AG_MutexUnlock(&agTextLock);
}

static __inline__ void
AG_TextTabWidth(int px)
{
	AG_MutexLock(&agTextLock);
	agTextState->tabWd = px;
	AG_MutexUnlock(&agTextLock);
}
#ifdef AG_LEGACY
#define AG_TextFormat AG_TextRenderf

static __inline__ void
AG_TextColor32(Uint32 px)
{
	AG_Color C;

	C = AG_GetColorRGB(px, agSurfaceFmt);
	AG_MutexLock(&agTextLock);
	agTextState->color = C;
	AG_MutexUnlock(&agTextLock);
}
#endif 
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif	/* _AGAR_GUI_TEXT_H_ */
