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
package = newpackage()
package.name = "ag_dev_static"
package.kind = "lib"
package.language = "c"
package.guid = "e0baea2d-5cb8-4a7a-9120-8352f36109e7"
dofile("../configure.lua")
tinsert(package.links,{"ag_core_static"})
tinsert(package.links,{"pthreadVC2"})
tinsert(package.links,{"winmm"})
tinsert(package.links,{"wsock32"})
tinsert(package.links,{"ws2_32"})
tinsert(package.links,{"iphlpapi"})
tinsert(package.links,{"ag_gui_static"})
tinsert(package.links,{"SDL"})
tinsert(package.links,{"freetype"})
tinsert(package.links,{"opengl32"})
package.files = {
	"dev.c",
	"browser.c",
	"timeouts.c",
	"uniconv.c",
	"view_params.c",
	"cpuinfo.c",
	"config.c",
	"object.c",
	"classes.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_DEV_INTERNAL"})
package = newpackage()
package.name = "ag_dev"
package.kind = "dll"
package.language = "c"
package.guid = "e0baea2d-5cb8-4a7a-9120-8352f36109e7"
dofile("../configure.lua")
tinsert(package.links,{"ag_core_static"})
tinsert(package.links,{"pthreadVC2"})
tinsert(package.links,{"winmm"})
tinsert(package.links,{"wsock32"})
tinsert(package.links,{"ws2_32"})
tinsert(package.links,{"iphlpapi"})
tinsert(package.links,{"ag_gui_static"})
tinsert(package.links,{"SDL"})
tinsert(package.links,{"freetype"})
tinsert(package.links,{"opengl32"})
package.files = {
	"dev.c",
	"browser.c",
	"timeouts.c",
	"uniconv.c",
	"view_params.c",
	"cpuinfo.c",
	"config.c",
	"object.c",
	"classes.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_DEV_INTERNAL"})
