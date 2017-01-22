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
package.name = "ag_vg_static"
package.kind = "lib"
package.language = "c"
package.guid = "6d009edb-c66a-46e8-a1dd-b6f653db6327"
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
	"vg.c",
	"vg_circle.c",
	"vg_arc.c",
	"vg_line.c",
	"vg_ortho.c",
	"vg_point.c",
	"vg_snap.c",
	"vg_tables.c",
	"vg_text.c",
	"vg_polygon.c",
	"vg_view.c",
	"vg_tool.c",
	"vg_circle_tool.c",
	"vg_line_tool.c",
	"vg_point_tool.c",
	"vg_proximity_tool.c",
	"vg_text_tool.c",
	"vg_arc_tool.c",
	"vg_polygon_tool.c",
	"vg_select_tool.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_VG_INTERNAL"})
package = newpackage()
package.name = "ag_vg"
package.kind = "dll"
package.language = "c"
package.guid = "6d009edb-c66a-46e8-a1dd-b6f653db6327"
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
	"vg.c",
	"vg_circle.c",
	"vg_arc.c",
	"vg_line.c",
	"vg_ortho.c",
	"vg_point.c",
	"vg_snap.c",
	"vg_tables.c",
	"vg_text.c",
	"vg_polygon.c",
	"vg_view.c",
	"vg_tool.c",
	"vg_circle_tool.c",
	"vg_line_tool.c",
	"vg_point_tool.c",
	"vg_proximity_tool.c",
	"vg_text_tool.c",
	"vg_arc_tool.c",
	"vg_polygon_tool.c",
	"vg_select_tool.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_VG_INTERNAL"})
