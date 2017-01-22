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
package.name = "ag_core_static"
package.kind = "lib"
package.language = "c"
package.guid = "9d6bb497-1d2a-4ac0-9f7b-7be68b9077e8"
dofile("../configure.lua")
tinsert(package.links,{"pthreadVC2"})
tinsert(package.links,{"winmm"})
tinsert(package.links,{"wsock32"})
tinsert(package.links,{"ws2_32"})
tinsert(package.links,{"iphlpapi"})
package.files = {
	"snprintf.c",
	"time_win32.c",
	"net.c",
	"net_dummy.c",
	"net_winsock1.c",
	"net_winsock2.c",
	"variable.c",
	"config.c",
	"core.c",
	"error.c",
	"event.c",
	"object.c",
	"prop.c",
	"timeout.c",
	"class.c",
	"cpuinfo.c",
	"data_source.c",
	"load_string.c",
	"load_version.c",
	"vsnprintf.c",
	"vasprintf.c",
	"asprintf.c",
	"dir.c",
	"md5.c",
	"sha1.c",
	"rmd160.c",
	"file.c",
	"string.c",
	"dso.c",
	"tree.c",
	"time.c",
	"time_dummy.c",
	"db.c",
	"tbl.c",
	"getopt.c",
	"exec.c",
	"text.c",
	"user.c",
	"user_dummy.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_CORE_INTERNAL"})
package = newpackage()
package.name = "ag_core"
package.kind = "dll"
package.language = "c"
package.guid = "9d6bb497-1d2a-4ac0-9f7b-7be68b9077e8"
dofile("../configure.lua")
tinsert(package.links,{"pthreadVC2"})
tinsert(package.links,{"winmm"})
tinsert(package.links,{"wsock32"})
tinsert(package.links,{"ws2_32"})
tinsert(package.links,{"iphlpapi"})
package.files = {
	"snprintf.c",
	"time_win32.c",
	"net.c",
	"net_dummy.c",
	"net_winsock1.c",
	"net_winsock2.c",
	"variable.c",
	"config.c",
	"core.c",
	"error.c",
	"event.c",
	"object.c",
	"prop.c",
	"timeout.c",
	"class.c",
	"cpuinfo.c",
	"data_source.c",
	"load_string.c",
	"load_version.c",
	"vsnprintf.c",
	"vasprintf.c",
	"asprintf.c",
	"dir.c",
	"md5.c",
	"sha1.c",
	"rmd160.c",
	"file.c",
	"string.c",
	"dso.c",
	"tree.c",
	"time.c",
	"time_dummy.c",
	"db.c",
	"tbl.c",
	"getopt.c",
	"exec.c",
	"text.c",
	"user.c",
	"user_dummy.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_CORE_INTERNAL"})
