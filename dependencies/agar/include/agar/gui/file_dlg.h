/*	Public domain	*/

#ifndef _AGAR_WIDGET_FILE_DLG_H_
#define _AGAR_WIDGET_FILE_DLG_H_

#include <agar/gui/widget.h>
#include <agar/gui/textbox.h>
#include <agar/gui/button.h>
#include <agar/gui/window.h>
#include <agar/gui/tlist.h>
#include <agar/gui/combo.h>
#include <agar/gui/checkbox.h>
#include <agar/gui/pane.h>
#include <agar/gui/label.h>

#include <agar/gui/begin.h>

struct ag_file_dlg;

enum ag_file_type_option_type {
	AG_FILEDLG_BOOL,
	AG_FILEDLG_INT,
	AG_FILEDLG_FLOAT,
	AG_FILEDLG_DOUBLE,
	AG_FILEDLG_STRING
};

typedef struct ag_file_type_option {
	const char *descr;
	const char *key;
	const char *unit;
	enum ag_file_type_option_type type;
	union {
		struct { int val, min, max; } i;
		struct { float val, min, max; } flt;
		struct { double val, min, max; } dbl;
		char s[128];
	} data;
	AG_TAILQ_ENTRY(ag_file_type_option) opts;
} AG_FileOption;

typedef struct ag_file_type {
	struct ag_file_dlg *fd;
	const char *descr;			/* Description */
	char **exts;				/* Filename extensions */
	Uint nexts;
	AG_Function *action;			  /* Action (save/load) */
	AG_TAILQ_HEAD_(ag_file_type_option) opts; /* Type-specific options */
	AG_TAILQ_ENTRY(ag_file_type) types;
} AG_FileType;

typedef struct ag_file_dlg {
	AG_Widget wid;
	Uint flags;
#define AG_FILEDLG_MULTI	  0x0001	/* Return a set of files */
#define AG_FILEDLG_CLOSEWIN	  0x0002	/* Close parent window */
#define AG_FILEDLG_LOAD		  0x0004	/* File must exist and be readable */
#define AG_FILEDLG_SAVE		  0x0008	/* File must be writeable */
#define AG_FILEDLG_ASYNC	  0x0010	/* Separate thread for load/save fn */
#define AG_FILEDLG_RESET_ONSHOW	  0x0020	/* Reset listing/locations on show */
#define AG_FILEDLG_HFILL	  0x0100
#define AG_FILEDLG_VFILL	  0x0200
#define AG_FILEDLG_EXPAND	  (AG_FILEDLG_HFILL|AG_FILEDLG_VFILL)
#define AG_FILEDLG_NOBUTTONS	  0x0400	/* No OK/Cancel buttons */
#define AG_FILEDLG_MASK_EXT	  0x0800	/* Mask files by extension */
#define AG_FILEDLG_MASK_HIDDEN	  0x1000	/* Mask hidden files */
#define AG_FILEDLG_NOMASKOPTS	  0x2000	/* No "Mask files" checkboxes */
#define AG_FILEDLG_NOTYPESELECT	  0x4000	/* No "Type" dropbox */

	char cwd[AG_PATHNAME_MAX];		/* Current working directory */
	char cfile[AG_PATHNAME_MAX];		/* Current file path */
	AG_Pane *hPane;
	AG_Tlist *tlDirs;			/* List of directories */
	AG_Tlist *tlFiles;			/* List of files */
	AG_Label *lbCwd;
	AG_Textbox *tbFile;
	AG_Combo *comTypes;
	AG_Checkbox *cbMaskExt, *cbMaskHidden;
	AG_Button *btnOk, *btnCancel;
	AG_Event *okAction, *cancelAction;
	char *dirMRU;				/* MRU Directory */
	void *optsCtr;				/* Container widget for opts */
	AG_TAILQ_HEAD_(ag_file_type) types;	/* File type handlers */
	AG_Combo *comLoc;			/* Locations list */
} AG_FileDlg;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agFileDlgClass;
extern DECLSPEC AG_FileDlg *AG_FileDlgNew(void *, Uint);
extern DECLSPEC AG_FileDlg *AG_FileDlgNewMRU(void *, const char *, Uint);
extern DECLSPEC void AG_FileDlgSetOptionContainer(AG_FileDlg *, void *);
extern DECLSPEC void AG_FileDlgSetDirectoryMRU(AG_FileDlg *, const char *, const char *);
extern DECLSPEC int AG_FileDlgSetDirectoryS(AG_FileDlg *, const char *);
extern DECLSPEC int AG_FileDlgSetDirectory(AG_FileDlg *, const char *, ...) FORMAT_ATTRIBUTE(printf,2,3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_FileDlgSetFilenameS(AG_FileDlg *, const char *);
extern DECLSPEC void AG_FileDlgSetFilename(AG_FileDlg *, const char *, ...) FORMAT_ATTRIBUTE(printf,2,3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_FileDlgOkAction(AG_FileDlg *, AG_EventFn, const char *, ...);
extern DECLSPEC void AG_FileDlgCancelAction(AG_FileDlg *, AG_EventFn, const char *, ...);
extern DECLSPEC int AG_FileDlgCheckReadAccess(AG_FileDlg *);
extern DECLSPEC int AG_FileDlgCheckWriteAccess(AG_FileDlg *);
extern DECLSPEC void AG_FileDlgRefresh(AG_FileDlg *);
extern DECLSPEC AG_FileType *AG_FileDlgAddType(AG_FileDlg *, const char *, const char *, AG_IntFn, const char *, ...);
extern DECLSPEC AG_FileOption *AG_FileOptionNewBool(AG_FileType *, const char *, const char *, int);
extern DECLSPEC AG_FileOption *AG_FileOptionNewInt(AG_FileType *, const char *, const char *, int, int, int);
extern DECLSPEC AG_FileOption *AG_FileOptionNewFlt(AG_FileType *, const char *, const char *, float, float, float, const char *);
extern DECLSPEC AG_FileOption *AG_FileOptionNewDbl(AG_FileType *, const char *, const char *, double, double, double, const char *);
extern DECLSPEC AG_FileOption *AG_FileOptionNewString(AG_FileType *, const char *, const char *, const char *);
extern DECLSPEC AG_FileOption *AG_FileOptionGet(AG_FileType *, const char *);
extern DECLSPEC int AG_FileOptionInt(AG_FileType *, const char *);
#define AG_FileOptionBool(ft,key) AG_FileOptionInt((ft),(key))
extern DECLSPEC float AG_FileOptionFlt(AG_FileType *, const char *);
extern DECLSPEC double AG_FileOptionDbl(AG_FileType *, const char *);
extern DECLSPEC char *AG_FileOptionString(AG_FileType *, const char *);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_FILE_DLG_H_ */
