/*	Public domain	*/

#ifndef _AGAR_GUI_GUI_H_
#define _AGAR_GUI_GUI_H_

#include <agar/config/have_sdl.h>
#include <agar/gui/begin.h>

#define AG_ZOOM_RANGE	14		/* Number of default zoom settings */
#define AG_ZOOM_DEFAULT	5		/* Initial zoom setting (1.0) */

/* For old AG_InitVideo() interface */
#define AG_VIDEO_HWSURFACE     0x0001
#define AG_VIDEO_ASYNCBLIT     0x0002
#define AG_VIDEO_ANYFORMAT     0x0004
#define AG_VIDEO_HWPALETTE     0x0008
#define AG_VIDEO_DOUBLEBUF     0x0010
#define AG_VIDEO_FULLSCREEN    0x0020
#define AG_VIDEO_RESIZABLE     0x0040
#define AG_VIDEO_NOFRAME       0x0080
#define AG_VIDEO_BGPOPUPMENU   0x0100
#define AG_VIDEO_OPENGL	       0x0200
#define AG_VIDEO_OPENGL_OR_SDL 0x0400
#define AG_VIDEO_NOBGCLEAR     0x0800
#define AG_VIDEO_OVERLAY       0x1000
#define AG_VIDEO_SDL           0x2000

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC int agGUI, agRenderingContext, agStereo;
extern DECLSPEC int agKbdDelay, agKbdRepeat;
extern DECLSPEC int agMouseDblclickDelay, agMouseSpinDelay, agMouseSpinIval, agMouseScrollDelay, agMouseScrollIval;
extern DECLSPEC int agTextComposition, agTextBidi, agTextCache, agTextTabWidth, agTextBlinkRate, agTextSymbols, agPageIncrement;
extern DECLSPEC int agIdleThresh, agScreenshotQuality, agMsgDelay;
extern DECLSPEC double agZoomValues[AG_ZOOM_RANGE];
extern DECLSPEC int AG_InitGraphics(const char *);
extern DECLSPEC void AG_DestroyGraphics(void);
extern DECLSPEC int AG_InitGUI(Uint);
extern DECLSPEC void AG_DestroyGUI(void);
extern DECLSPEC int AG_InitGUIGlobals(void);
extern DECLSPEC void AG_DestroyGUIGlobals(void);
extern DECLSPEC void AG_QuitGUI(void);
extern DECLSPEC void AG_ZoomIn(void);
extern DECLSPEC void AG_ZoomOut(void);
extern DECLSPEC void AG_ZoomReset(void);
#ifdef AG_DEBUG
extern DECLSPEC void *AG_GuiDebugger(void *);
#endif
#ifdef HAVE_SDL
extern DECLSPEC int AG_InitVideoSDL(void *, Uint);
extern DECLSPEC int AG_SetVideoSurfaceSDL(void *);
#endif
#ifdef AG_LEGACY
extern DECLSPEC int AG_InitVideo(int, int, int, Uint) DEPRECATED_ATTRIBUTE;
extern DECLSPEC void AG_DestroyVideo(void) DEPRECATED_ATTRIBUTE;
#endif
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_GUI_GUI_H_ */
