/*	Public domain	*/

#ifndef _AGAR_WIDGET_MSPINBUTTON_H_
#define _AGAR_WIDGET_MSPINBUTTON_H_

#include <agar/gui/widget.h>
#include <agar/gui/textbox.h>
#include <agar/gui/button.h>

#include <agar/gui/begin.h>

#define AG_MSPINBUTTON_NOHFILL	0x01
#define AG_MSPINBUTTON_VFILL	0x02

typedef struct ag_mspinbutton {
	struct ag_widget wid;
	const char *sep;			/* x/y value separator */
	int xvalue, yvalue;			/* Default x/y bindings */
	int min, max;				/* Default range bindings */
	int inc;				/* Increment for buttons */
	int writeable;				/* 0 = read-only */
	char inTxt[64];				/* Input text buffer */
	AG_Textbox *input;
	AG_Button *xincbu, *xdecbu;
	AG_Button *yincbu, *ydecbu;
} AG_MSpinbutton;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agMSpinbuttonClass;
extern DECLSPEC AG_MSpinbutton *AG_MSpinbuttonNew(void *, Uint, const char *, const char *);
extern DECLSPEC void AG_MSpinbuttonAddValue(AG_MSpinbutton *, const char *, int);
extern DECLSPEC void AG_MSpinbuttonSetValue(AG_MSpinbutton *, const char *, ...);
extern DECLSPEC void AG_MSpinbuttonSetMin(AG_MSpinbutton *, int);
extern DECLSPEC void AG_MSpinbuttonSetMax(AG_MSpinbutton *, int);
extern DECLSPEC void AG_MSpinbuttonSetRange(AG_MSpinbutton *, int, int);
extern DECLSPEC void AG_MSpinbuttonSetIncrement(AG_MSpinbutton *, int);
extern DECLSPEC void AG_MSpinbuttonSetWriteable(AG_MSpinbutton *, int);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_MSPINBUTTON_H_ */
