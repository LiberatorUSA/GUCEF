--
-- Do not edit!
-- This file was generated from Makefile by BSDbuild 3.2.
--
-- To regenerate this file, get the latest BSDbuild release from
-- http://hypertriton.com/bsdbuild/, the latest Premake release
-- (v3 series) from http://premake.sourceforge.net/, and execute:
--
--     $ make proj
--
project.name = "agartest"
package = newpackage()
package.name = "agartest"
package.kind = "winexe"
package.language = "c"
package.guid = "11D6C9FF-522E-43ed-B3EB-92A2C636CCA7"
dofile("../configure.lua")
tinsert(package.links,{"ag_math"})
tinsert(package.links,{"ag_dev"})
tinsert(package.links,{"ag_gui"})
tinsert(package.links,{"SDL"})
tinsert(package.links,{"freetype"})
tinsert(package.links,{"opengl32"})
tinsert(package.links,{"ag_core"})
tinsert(package.links,{"pthreadVC2"})
tinsert(package.links,{"winmm"})
tinsert(package.links,{"wsock32"})
tinsert(package.links,{"ws2_32"})
tinsert(package.links,{"iphlpapi"})
package.files = {
	"agartest.c",
	"charsets.c",
	"compositing.c",
	"configsettings.c",
	"console.c",
	"customwidget.c",
	"customwidget_mywidget.c",
	"fixedres.c",
	"focusing.c",
	"fontselector.c",
	"fspaths.c",
	"glview.c",
	"imageloading.c",
	"keyevents.c",
	"loader.c",
	"math.c",
	"maximized.c",
	"minimal.c",
	"modalwindowhandler.c",
	"network.c",
	"objsystem.c",
	"objsystem_animal.c",
	"objsystem_mammal.c",
	"pane.c",
	"palette.c",
	"plotting.c",
	"rendertosurface.c",
	"scrollbar.c",
	"scrollview.c",
	"sockets.c",
	"string.c",
	"table.c",
	"textbox.c",
	"textdlg.c",
	"threads.c",
	"timeouts.c",
	"unitconv.c",
	"widgets.c",
	"windows.c",
}
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/tests"})
tinsert(package.defines,{"_USE_AGAR_STD"})
tinsert(package.defines,{"_USE_AGAR_QUEUE"})
tinsert(package.defines,{"_USE_AGAR_TYPES"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/tests"})
