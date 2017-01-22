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
package.name = "ag_math_static"
package.kind = "lib"
package.language = "c"
package.guid = "ba35a553-bd88-4e5e-8c4b-d17f60ae7439"
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
	"m_math.c",
	"m_complex.c",
	"m_quaternion.c",
	"m_vector.c",
	"m_vectorz.c",
	"m_vector_fpu.c",
	"m_vector2_fpu.c",
	"m_vector3_fpu.c",
	"m_vector4_fpu.c",
	"m_vector3_sse.c",
	"m_matrix.c",
	"m_matrix_fpu.c",
	"m_matrix44_fpu.c",
	"m_matrix44_sse.c",
	"m_gui.c",
	"m_plotter.c",
	"m_matview.c",
	"m_line.c",
	"m_circle.c",
	"m_triangle.c",
	"m_rectangle.c",
	"m_polygon.c",
	"m_plane.c",
	"m_coordinates.c",
	"m_heapsort.c",
	"m_mergesort.c",
	"m_qsort.c",
	"m_radixsort.c",
	"m_point_set.c",
	"m_color.c",
	"m_sphere.c",
	"m_polyhedron.c",
	"m_matrix_sparse.c",
	"m_sparse_allocate.c",
	"m_sparse_build.c",
	"m_sparse_eda.c",
	"m_sparse_factor.c",
	"m_sparse_output.c",
	"m_sparse_solve.c",
	"m_sparse_utils.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_MATH_INTERNAL"})
package = newpackage()
package.name = "ag_math"
package.kind = "dll"
package.language = "c"
package.guid = "ba35a553-bd88-4e5e-8c4b-d17f60ae7439"
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
	"m_math.c",
	"m_complex.c",
	"m_quaternion.c",
	"m_vector.c",
	"m_vectorz.c",
	"m_vector_fpu.c",
	"m_vector2_fpu.c",
	"m_vector3_fpu.c",
	"m_vector4_fpu.c",
	"m_vector3_sse.c",
	"m_matrix.c",
	"m_matrix_fpu.c",
	"m_matrix44_fpu.c",
	"m_matrix44_sse.c",
	"m_gui.c",
	"m_plotter.c",
	"m_matview.c",
	"m_line.c",
	"m_circle.c",
	"m_triangle.c",
	"m_rectangle.c",
	"m_polygon.c",
	"m_plane.c",
	"m_coordinates.c",
	"m_heapsort.c",
	"m_mergesort.c",
	"m_qsort.c",
	"m_radixsort.c",
	"m_point_set.c",
	"m_color.c",
	"m_sphere.c",
	"m_polyhedron.c",
	"m_matrix_sparse.c",
	"m_sparse_allocate.c",
	"m_sparse_build.c",
	"m_sparse_eda.c",
	"m_sparse_factor.c",
	"m_sparse_output.c",
	"m_sparse_solve.c",
	"m_sparse_utils.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.5.0/include"})
tinsert(package.defines,{"_AGAR_MATH_INTERNAL"})
