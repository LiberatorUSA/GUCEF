--------------------------------------------------------------------
-- This file was automatically generated by ProjectGenerator
-- which is tooling part the build system designed for GUCEF
--     (Galaxy Unlimited Framework)
-- For the latest info, see http://www.VanvelzenSoftware.com/
--
-- The contents of this file are placed in the public domain. Feel
-- free to make use of it in any way you like.
--------------------------------------------------------------------
--

-- Configuration for module: DevIL2


project( "DevIL2" )

configuration( {} )
  location( os.getenv( "PM5OUTPUTDIR" ) )

configuration( {} )
  targetdir( os.getenv( "PM5TARGETDIR" ) )

configuration( {} )
language( "C++" )



configuration( {} )
kind( "SharedLib" )

configuration( {} )
links( { "OpenEXR", "jasper", "lcms2", "libjpeg", "libmng", "libpng", "libtiff", "squish", "zlib" } )
links( { "OpenEXR", "jasper", "lcms2", "libjpeg", "libmng", "libpng", "libtiff", "squish", "zlib" } )


configuration( {} )
defines( { "CMS_DLL", "HAVE_CONFIG_H", "JAS_DLL", "MNG_DLL", "MNG_USE_DLL" } )


configuration( {} )
vpaths { ["Headers"] = { "**.h", "**.hpp", "**.hxx" } }
files( {
  "src-IL/include/altivec_common.h",
  "src-IL/include/altivec_typeconversion.h",
  "src-IL/include/il_alloc.h",
  "src-IL/include/il_bits.h",
  "src-IL/include/il_bmp.h",
  "src-IL/include/il_dcx.h",
  "src-IL/include/il_dds.h",
  "src-IL/include/il_doompal.h",
  "src-IL/include/il_dpx.h",
  "src-IL/include/il_endian.h",
  "src-IL/include/il_exr.h",
  "src-IL/include/il_files.h",
  "src-IL/include/il_gif.h",
  "src-IL/include/il_hdr.h",
  "src-IL/include/il_icns.h",
  "src-IL/include/il_icon.h",
  "src-IL/include/il_internal.h",
  "src-IL/include/il_jp2.h",
  "src-IL/include/il_jpeg.h",
  "src-IL/include/il_lif.h",
  "src-IL/include/il_manip.h",
  "src-IL/include/il_mdl.h",
  "src-IL/include/il_pal.h",
  "src-IL/include/il_pcx.h",
  "src-IL/include/il_pic.h",
  "src-IL/include/il_pnm.h",
  "src-IL/include/il_psd.h",
  "src-IL/include/il_psp.h",
  "src-IL/include/il_q2pal.h",
  "src-IL/include/il_register.h",
  "src-IL/include/il_rle.h",
  "src-IL/include/il_sgi.h",
  "src-IL/include/il_stack.h",
  "src-IL/include/il_states.h",
  "src-IL/include/il_targa.h",
  "src-IL/include/il_utx.h",
  "src-IL/include/il_vtf.h",
  "src-IL/include/il_wdp.h",
  "src-IL/include/rg_etc1.h",
  "src-ILU/include/ilu_alloc.h",
  "src-ILU/include/ilu_filter.h",
  "src-ILU/include/ilu_internal.h",
  "src-ILU/include/ilu_region.h",
  "src-ILU/include/ilu_states.h",
  "src-ILU/include/ilu_error/ilu_err-arabic.h",
  "src-ILU/include/ilu_error/ilu_err-dutch.h",
  "src-ILU/include/ilu_error/ilu_err-english.h",
  "src-ILU/include/ilu_error/ilu_err-french.h",
  "src-ILU/include/ilu_error/ilu_err-german.h",
  "src-ILU/include/ilu_error/ilu_err-italian.h",
  "src-ILU/include/ilu_error/ilu_err-japanese.h",
  "src-ILU/include/ilu_error/ilu_err-spanish.h",
  "src-ILUT/include/ilut_allegro.h",
  "src-ILUT/include/ilut_internal.h",
  "src-ILUT/include/ilut_opengl.h",
  "src-ILUT/include/ilut_states.h"
 } )



configuration( {} )
vpaths { ["Source"] = { "**.c", "**.cpp", "**.cs", "**.asm" } }
files( {
  "src-IL/src/altivec_common.cpp",
  "src-IL/src/altivec_typeconversion.cpp",
  "src-IL/src/il_alloc.cpp",
  "src-IL/src/il_bits.cpp",
  "src-IL/src/il_blp.cpp",
  "src-IL/src/il_bmp.cpp",
  "src-IL/src/il_convbuff.cpp",
  "src-IL/src/il_convert.cpp",
  "src-IL/src/il_cut.cpp",
  "src-IL/src/il_dcx.cpp",
  "src-IL/src/il_dds-save.cpp",
  "src-IL/src/il_dds.cpp",
  "src-IL/src/il_devil.cpp",
  "src-IL/src/il_dicom.cpp",
  "src-IL/src/il_doom.cpp",
  "src-IL/src/il_dpx.cpp",
  "src-IL/src/il_endian.cpp",
  "src-IL/src/il_error.cpp",
  "src-IL/src/il_exr.c",
  "src-IL/src/il_exr.cpp",
  "src-IL/src/il_fastconv.cpp",
  "src-IL/src/il_files.cpp",
  "src-IL/src/il_fits.cpp",
  "src-IL/src/il_ftx.cpp",
  "src-IL/src/il_gif.cpp",
  "src-IL/src/il_hdr.cpp",
  "src-IL/src/il_header.cpp",
  "src-IL/src/il_icns.cpp",
  "src-IL/src/il_icon.cpp",
  "src-IL/src/il_iff.cpp",
  "src-IL/src/il_ilbm.cpp",
  "src-IL/src/il_internal.cpp",
  "src-IL/src/il_io.cpp",
  "src-IL/src/il_iwi.cpp",
  "src-IL/src/il_jp2.cpp",
  "src-IL/src/il_jpeg.cpp",
  "src-IL/src/il_ktx.cpp",
  "src-IL/src/il_lif.cpp",
  "src-IL/src/il_manip.cpp",
  "src-IL/src/il_mdl.cpp",
  "src-IL/src/il_mng.cpp",
  "src-IL/src/il_mp3.cpp",
  "src-IL/src/il_neuquant.cpp",
  "src-IL/src/il_nvidia.cpp",
  "src-IL/src/il_pal.cpp",
  "src-IL/src/il_pcd.cpp",
  "src-IL/src/il_pcx.cpp",
  "src-IL/src/il_pic.cpp",
  "src-IL/src/il_pix.cpp",
  "src-IL/src/il_png.cpp",
  "src-IL/src/il_pnm.cpp",
  "src-IL/src/il_profiles.cpp",
  "src-IL/src/il_psd.cpp",
  "src-IL/src/il_psp.cpp",
  "src-IL/src/il_pxr.cpp",
  "src-IL/src/il_quantizer.cpp",
  "src-IL/src/il_raw.cpp",
  "src-IL/src/il_rawdata.cpp",
  "src-IL/src/il_register.cpp",
  "src-IL/src/il_rle.cpp",
  "src-IL/src/il_rot.cpp",
  "src-IL/src/il_sgi.cpp",
  "src-IL/src/il_size.cpp",
  "src-IL/src/il_squish.cpp",
  "src-IL/src/il_stack.cpp",
  "src-IL/src/il_states.cpp",
  "src-IL/src/il_sun.cpp",
  "src-IL/src/il_targa.cpp",
  "src-IL/src/il_texture.cpp",
  "src-IL/src/il_tiff.cpp",
  "src-IL/src/il_tpl.cpp",
  "src-IL/src/il_utility.cpp",
  "src-IL/src/il_utx.c",
  "src-IL/src/il_utx.cpp",
  "src-IL/src/il_vtf.cpp",
  "src-IL/src/il_wal.cpp",
  "src-IL/src/il_wbmp.cpp",
  "src-IL/src/il_wdp.cpp",
  "src-IL/src/il_xpm.cpp",
  "src-IL/src/rg_etc1.cpp",
  "src-ILU/src/ilu_alloc.cpp",
  "src-ILU/src/ilu_error.cpp",
  "src-ILU/src/ilu_filter.cpp",
  "src-ILU/src/ilu_filter_rcg.cpp",
  "src-ILU/src/ilu_internal.cpp",
  "src-ILU/src/ilu_manip.cpp",
  "src-ILU/src/ilu_mipmap.cpp",
  "src-ILU/src/ilu_noise.cpp",
  "src-ILU/src/ilu_region.cpp",
  "src-ILU/src/ilu_rotate.cpp",
  "src-ILU/src/ilu_scale.cpp",
  "src-ILU/src/ilu_scale2d.cpp",
  "src-ILU/src/ilu_scale3d.cpp",
  "src-ILU/src/ilu_scaling.cpp",
  "src-ILU/src/ilu_states.cpp",
  "src-ILU/src/ilu_utilities.cpp",
  "src-ILUT/src/ilut_allegro.cpp",
  "src-ILUT/src/ilut_directx.cpp",
  "src-ILUT/src/ilut_directx10.cpp",
  "src-ILUT/src/ilut_directx9.cpp",
  "src-ILUT/src/ilut_directxm.cpp",
  "src-ILUT/src/ilut_internal.cpp",
  "src-ILUT/src/ilut_opengl.cpp",
  "src-ILUT/src/ilut_sdlsurface.cpp",
  "src-ILUT/src/ilut_states.cpp",
  "src-ILUT/src/ilut_win32.cpp",
  "src-ILUT/src/ilut_x11.cpp"
 } )


configuration( {} )
includedirs( { "../../Little-CMS/include", "../../Little-CMS/src", "../../jasper/src/libjasper/bmp", "../../jasper/src/libjasper/include", "../../jasper/src/libjasper/include/jasper", "../../jasper/src/libjasper/jp2", "../../jasper/src/libjasper/jpc", "../../jasper/src/libjasper/jpg", "../../jasper/src/libjasper/mif", "../../jasper/src/libjasper/pgx", "../../jasper/src/libjasper/pnm", "../../jasper/src/libjasper/ras", "../../libjpeg", "../../libmng", "../../libpng", "../../libsquish", "../../openexr/IlmBase", "../../openexr/IlmBase/Half", "../../openexr/IlmBase/Iex", "../../openexr/IlmBase/IexMath", "../../openexr/IlmBase/IlmThread", "../../openexr/IlmBase/Imath", "../../openexr/IlmBase/config.windows", "../../openexr/OpenEXR", "../../openexr/OpenEXR/IlmImf", "../../openexr/OpenEXR/IlmImfUtil", "../../openexr/OpenEXR/config.windows", "../../tiff/libtiff", "include", "include/IL", "src-IL", "src-ILU", "src-ILUT", "src-IL/include", "src-ILU/include", "src-ILU/include/ilu_error", "src-ILUT/include" } )

configuration( { "LINUX32" } )
includedirs( { "../../zlib", "include/IL/linux" } )

configuration( { "LINUX64" } )
includedirs( { "../../zlib", "include/IL/linux" } )

configuration( { "WIN32" } )
includedirs( { "../../zlib", "include/IL/mswin" } )

configuration( { "WIN64" } )
includedirs( { "../../zlib", "include/IL/mswin" } )