/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef GLDRIVER_H
#define GLDRIVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD CONFIG                                                       //
//                                                                         //
//-------------------------------------------------------------------------*/

/* #define GLDRIVER_BUILD_MODULE */            /* enable this flag if you are building GLDriver instead of linking to it */
#define GLDRIVER_MSWIN_BUILD                   /* enable this flag if you are buidling for the MS Windows platform */
/* #define GLDRIVER_DEBUG_MODE */              /* enable this flag if you wish to debug your OpenGL code */
/* #define GLDRIVER_DYNAMIC_OPENGL_LINKING */  /* enable this flag if you wish to link the OpenGL library dynamicly. Note that you must ensure you don't include OpenGL yourself */
/* #define GLDRIVER_EXPORTS_GLAPI */           /* enables the export of the here defined OpenGL functions */
/* #define GLDRIVER_EXPORTS_DEBUG_GLAPI */     /* enables the export of the here defined OpenGL debug functions */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GLDRIVER_DYNAMIC_OPENGL_LINKING

/*
 *      Macro to make sure the user doesnt include gl.h before this header.
 */
#if defined( __gl_h_ ) || defined( __GL_H__ )
#error DO NOT INCLUDE OPENGL HEADERS YOURSELF !!!
#endif

#define __gl_h_
#define __GL_H__

#endif /* GLDRIVER_DYNAMIC_OPENGL_LINKING ? */

/*--------------------------------------------------------------------------*/

/*
 *   OpenGL uses the standard calling convention.
 *   The following macro will adapt the syntax to your compiler   
 */
#undef GLDRIVER_CALLSPEC_PREFIX
#undef GLDRIVER_CALLSPEC_SUFFIX  
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GLDRIVER_CALLSPEC_PREFIX __stdcall
  #define GLDRIVER_CALLSPEC_SUFFIX
#else
  #define GLDRIVER_CALLSPEC_PREFIX
  #define GLDRIVER_CALLSPEC_SUFFIX  __attribute__((stdcall))
#endif

/*--------------------------------------------------------------------------*/

/*
 *  Adjust the export macro to match the platform standard
 */
#undef GLDRIVER_EXPORT
#ifdef GLDRIVER_BUILD_MODULE
  #ifdef GLDRIVER_MSWIN_BUILD
    #define GLDRIVER_EXPORT __declspec( dllexport )  
  #else 
    #define GLDRIVER_EXPORT /* not needed under Linux */
  #endif
#else
  #ifdef GLDRIVER_MSWIN_BUILD
    #define GLDRIVER_EXPORT __declspec( dllimport )  
  #else 
    #define GLDRIVER_EXPORT /* not needed under Linux */
  #endif
#endif  

/*--------------------------------------------------------------------------*/

#undef GLDRIVER_EXPORT_C
#ifdef GLDRIVER_EXPORTS_GLAPI /* define this if you wish to export the OpenGL API functions */
  #define GLDRIVER_EXPORT_C GLDRIVER_EXPORT  
#else
  #define GLDRIVER_EXPORT_C
#endif

/*--------------------------------------------------------------------------*/

#undef GLDRIVER_DEBUG_EXPORT_C
#ifdef GLDRIVER_EXPORTS_DEBUG_GLAPI  /* define this if you wish to export the debug OpenGL API functions */
  #define GLDRIVER_DEBUG_EXPORT_C GLDRIVER_EXPORT  
#else
  #define GLDRIVER_DEBUG_EXPORT_C
#endif

/*--------------------------------------------------------------------------*/

/*
 *  define the marhalled types here. These are undefined at the bottom of this
 *  file
 */
#define UInt32  unsigned long

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/* Only define contants ourselves if we are dynamicly linking OpenGL */
#ifdef GLDRIVER_DYNAMIC_OPENGL_LINKING

/*--------------------------------------------------------------------------*/

/*
 *      defines indicating the currently supported OpenGL versions.
 */
#define GL_VERSION_1_1
#define GL_VERSION_1_2
#define GL_VERSION_1_3
#define GL_VERSION_1_4
#define GL_VERSION_1_5

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.1 constants
 */
#define GL_ACCUM 0x0100
#define GL_LOAD 0x0101
#define GL_RETURN 0x0102
#define GL_MULT 0x0103
#define GL_ADD 0x0104
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_CURRENT_BIT 0x00000001
#define GL_POINT_BIT 0x00000002
#define GL_LINE_BIT 0x00000004
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_LIGHTING_BIT 0x00000040
#define GL_FOG_BIT 0x00000080
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_VIEWPORT_BIT 0x00000800
#define GL_TRANSFORM_BIT 0x00001000
#define GL_ENABLE_BIT 0x00002000
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_HINT_BIT 0x00008000
#define GL_EVAL_BIT 0x00010000
#define GL_LIST_BIT 0x00020000
#define GL_TEXTURE_BIT 0x00040000
#define GL_SCISSOR_BIT 0x00080000
#define GL_ALL_ATTRIB_BITS 0x000fffff
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON 0x0009
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_TRUE 1
#define GL_FALSE 0
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_2_BYTES 0x1407
#define GL_3_BYTES 0x1408
#define GL_4_BYTES 0x1409
#define GL_DOUBLE 0x140A
#define GL_NONE 0
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_AUX0 0x0409
#define GL_AUX1 0x040A
#define GL_AUX2 0x040B
#define GL_AUX3 0x040C
#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505
#define GL_2D 0x0600
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_4D_COLOR_TEXTURE 0x0604
#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_POINT_TOKEN 0x0701
#define GL_LINE_TOKEN 0x0702
#define GL_POLYGON_TOKEN 0x0703
#define GL_BITMAP_TOKEN 0x0704
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_LINE_RESET_TOKEN 0x0707
#define GL_EXP 0x0800
#define GL_EXP2 0x0801
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_COEFF 0x0A00
#define GL_ORDER 0x0A01
#define GL_DOMAIN 0x0A02
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LIST_MODE 0x0B30
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_LIST_BASE 0x0B32
#define GL_LIST_INDEX 0x0B33
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_EDGE_FLAG 0x0B43
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_LIGHTING 0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_SHADE_MODEL 0x0B54
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_COLOR_MATERIAL 0x0B57
#define GL_FOG 0x0B60
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_FOG_MODE 0x0B65
#define GL_FOG_COLOR 0x0B66
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_MATRIX_MODE 0x0BA0
#define GL_NORMALIZE 0x0BA1
#define GL_VIEWPORT 0x0BA2
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_DITHER 0x0BD0
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND 0x0BE2
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_AUX_BUFFERS 0x0C00
#define GL_DRAW_BUFFER 0x0C01
#define GL_READ_BUFFER 0x0C02
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_INDEX_MODE 0x0C30
#define GL_RGBA_MODE 0x0C31
#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO 0x0C33
#define GL_RENDER_MODE 0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_FOG_HINT 0x0C54
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_STENCIL 0x0D11
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_OFFSET 0x0D13
#define GL_RED_SCALE 0x0D14
#define GL_RED_BIAS 0x0D15
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17
#define GL_GREEN_SCALE 0x0D18
#define GL_GREEN_BIAS 0x0D19
#define GL_BLUE_SCALE 0x0D1A
#define GL_BLUE_BIAS 0x0D1B
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_BIAS 0x0D1F
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_INDEX_BITS 0x0D51
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_ALPHA_BITS 0x0D55
#define GL_DEPTH_BITS 0x0D56
#define GL_STENCIL_BITS 0x0D57
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_NAME_STACK_DEPTH 0x0D70
#define GL_AUTO_NORMAL 0x0D80
#define GL_MAP1_COLOR_4 0x0D90
#define GL_MAP1_INDEX 0x0D91
#define GL_MAP1_NORMAL 0x0D92
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#define GL_MAP1_VERTEX_3 0x0D97
#define GL_MAP1_VERTEX_4 0x0D98
#define GL_MAP2_COLOR_4 0x0DB0
#define GL_MAP2_INDEX 0x0DB1
#define GL_MAP2_NORMAL 0x0DB2
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#define GL_MAP2_VERTEX_3 0x0DB7
#define GL_MAP2_VERTEX_4 0x0DB8
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#define GL_SELECTION_BUFFER_SIZE 0x0DF4
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_BORDER 0x1005
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007
#define GL_AMBIENT 0x1200
#define GL_DIFFUSE 0x1201
#define GL_SPECULAR 0x1202
#define GL_POSITION 0x1203
#define GL_SPOT_DIRECTION 0x1204
#define GL_SPOT_EXPONENT 0x1205
#define GL_SPOT_CUTOFF 0x1206
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301
#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
#define GL_SET 0x150F
#define GL_EMISSION 0x1600
#define GL_SHININESS 0x1601
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_COLOR_INDEXES 0x1603
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_COLOR_INDEX 0x1900
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_BITMAP 0x1A00
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_RENDER 0x1C00
#define GL_FEEDBACK 0x1C01
#define GL_SELECT 0x1C02
#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_S 0x2000
#define GL_T 0x2001
#define GL_R 0x2002
#define GL_Q 0x2003
#define GL_MODULATE 0x2100
#define GL_DECAL 0x2101
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_ENV 0x2300
#define GL_EYE_LINEAR 0x2400
#define GL_OBJECT_LINEAR 0x2401
#define GL_SPHERE_MAP 0x2402
#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_OBJECT_PLANE 0x2501
#define GL_EYE_PLANE 0x2502
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_CLIENT_ALL_ATTRIB_BITS 0xffffffff
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_INTENSITY 0x8049
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_R3_G3_B2 0x2A10
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB8 0x8051
#define GL_RGB10 0x8052
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_INDEX_ARRAY 0x8077
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_C3F_V3F 0x2A24
#define GL_N3F_V3F 0x2A25
#define GL_C4F_N3F_V3F 0x2A26
#define GL_T2F_V3F 0x2A27
#define GL_T4F_V4F 0x2A28
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#define GL_LOGIC_OP GL_INDEX_LOGIC_OP
#define GL_TEXTURE_COMPONENTS GL_TEXTURE_INTERNAL_FORMAT
#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.2 constants
 */
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_RESCALE_NORMAL 0x803A
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SINGLE_COLOR 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.3 constants
 */
#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_SUBTRACT 0x84E7
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_NORMAL_MAP 0x8511
#define GL_REFLECTION_MAP 0x8512
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE2_ALPHA 0x858A
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#define GL_MULTISAMPLE_BIT 0x20000000

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.4 constants
 */
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_MIRRORED_REPEAT 0x8370
#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORDINATE 0x8451
#define GL_FRAGMENT_DEPTH 0x8452
#define GL_CURRENT_FOG_COORDINATE 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#define GL_FOG_COORDINATE_ARRAY 0x8457
#define GL_COLOR_SUM 0x8458
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_COMPARE_R_TO_TEXTURE 0x884E

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.5 constants
 */
#define GL_FOG_COORD GL_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY GL_FOG_COORDINATE_ARRAY
#define GL_SRC0_RGB GL_SOURCE0_RGB
#define GL_FOG_COORD_ARRAY_POINTER GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_SOURCE GL_FOG_COORDINATE_SOURCE
#define GL_FOG_COORD_ARRAY_TYPE GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_SRC1_ALPHA GL_SOURCE1_ALPHA
#define GL_CURRENT_FOG_COORD GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY_STRIDE GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_SRC0_ALPHA GL_SOURCE0_ALPHA
#define GL_SRC1_RGB GL_SOURCE1_RGB
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_SRC2_ALPHA GL_SOURCE2_ALPHA
#define GL_SRC2_RGB GL_SOURCE2_RGB
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_SAMPLES_PASSED 0x8914

/*--------------------------------------------------------------------------*/

#endif /* GLDRIVER_DYNAMIC_OPENGL_LINKING ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Enumeration values indicating the possible return values of the
 *      glDriver functions defined in this header.
 */
enum
{
        GLDRIVER_UNABLE_TO_LOAD_DRIVER = -2,
        GLDRIVER_UNABLE_TO_LINK_FUNCTION,
        GLDRIVER_FALSE,
        GLDRIVER_TRUE = 1
};

/*--------------------------------------------------------------------------*/

#ifdef GLDRIVER_DYNAMIC_OPENGL_LINKING

/*
 *      OpenGL API Types.
 */
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

#endif /* GLDRIVER_DYNAMIC_OPENGL_LINKING ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      We can now safely include Glu since the GL header include gaurds have
 *      been defined.
 */
#ifdef GLDRIVER_MSWIN_BUILD
  #include <windows.h>
#endif /* GLDRIVER_MSWIN_BUILD ? */
#ifndef GLDRIVER_DYNAMIC_OPENGL_LINKING
  #include <GL/GL.h>
#endif /* GLDRIVER_DYNAMIC_OPENGL_LINKING ? */
#include <GL/GLu.h>

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  If we are dynamicly linking to the OpenGL header we will use function pointers
 *  to make the transition transparant
 */
#ifdef GLDRIVER_DYNAMIC_OPENGL_LINKING

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL 1.1 functions.
 *      Support for all of these functions is manditory for any driver you
 *      attempt to load. OpenGL versions older then 1.1 are not supported.
 */
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glAccum) (GLenum op, GLfloat value) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glAlphaFunc) (GLenum func, GLclampf ref) GLDRIVER_CALLSPEC_SUFFIX;
extern GLboolean GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glAreTexturesResident) (GLsizei n, const GLuint *textures, GLboolean *residences) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glArrayElement) (GLint i) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBegin) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBindTexture) (GLenum target, GLuint texture) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBitmap) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBlendFunc) (GLenum sfactor, GLenum dfactor) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCallList) (GLuint list) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCallLists) (GLsizei n, GLenum type, const GLvoid *lists) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClear) (GLbitfield mask) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClearAccum) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClearDepth) (GLclampd depth) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClearIndex) (GLfloat c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClearStencil) (GLint s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClipPlane) (GLenum plane, const GLdouble *equation) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3b) (GLbyte red, GLbyte green, GLbyte blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3bv) (const GLbyte *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3d) (GLdouble red, GLdouble green, GLdouble blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3f) (GLfloat red, GLfloat green, GLfloat blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3i) (GLint red, GLint green, GLint blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3s) (GLshort red, GLshort green, GLshort blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3ub) (GLubyte red, GLubyte green, GLubyte blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3ubv) (const GLubyte *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3ui) (GLuint red, GLuint green, GLuint blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3uiv) (const GLuint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3us) (GLushort red, GLushort green, GLushort blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor3usv) (const GLushort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4b) (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4bv) (const GLbyte *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4d) (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4i) (GLint red, GLint green, GLint blue, GLint alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4s) (GLshort red, GLshort green, GLshort blue, GLshort alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4ubv) (const GLubyte *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4ui) (GLuint red, GLuint green, GLuint blue, GLuint alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4uiv) (const GLuint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4us) (GLushort red, GLushort green, GLushort blue, GLushort alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColor4usv) (const GLushort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColorMaterial) (GLenum face, GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCopyTexImage1D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCopyTexImage2D) (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCopyTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCullFace) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDeleteLists) (GLuint list, GLsizei range) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDeleteTextures) (GLsizei n, const GLuint *textures) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDepthFunc) (GLenum func) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDepthMask) (GLboolean flag) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDepthRange) (GLclampd zNear, GLclampd zFar) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDisable) (GLenum cap) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDisableClientState) (GLenum array) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDrawArrays) (GLenum mode, GLint first, GLsizei count) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDrawBuffer) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEdgeFlag) (GLboolean flag) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEdgeFlagPointer) (GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEdgeFlagv) (const GLboolean *flag) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEnable) (GLenum cap) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEnableClientState) (GLenum array) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEnd) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEndList) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord1d) (GLdouble u) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord1dv) (const GLdouble *u) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord1f) (GLfloat u) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord1fv) (const GLfloat *u) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord2d) (GLdouble u, GLdouble v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord2dv) (const GLdouble *u) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord2f) (GLfloat u, GLfloat v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalCoord2fv) (const GLfloat *u) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalMesh1) (GLenum mode, GLint i1, GLint i2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalMesh2) (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalPoint1) (GLint i) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEvalPoint2) (GLint i, GLint j) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFeedbackBuffer) (GLsizei size, GLenum type, GLfloat *buffer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFinish) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFlush) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogf) (GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogfv) (GLenum pname, const GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogi) (GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogiv) (GLenum pname, const GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFrontFace) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) GLDRIVER_CALLSPEC_SUFFIX;
extern GLuint GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGenLists) (GLsizei range) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGenTextures) (GLsizei n, GLuint *textures) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetBooleanv) (GLenum pname, GLboolean *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetClipPlane) (GLenum plane, GLdouble *equation) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetDoublev) (GLenum pname, GLdouble *params) GLDRIVER_CALLSPEC_SUFFIX;
extern GLenum GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetError) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetFloatv) (GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetIntegerv) (GLenum pname, GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetLightfv) (GLenum light, GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetLightiv) (GLenum light, GLenum pname, GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetMapdv) (GLenum target, GLenum query, GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetMapfv) (GLenum target, GLenum query, GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetMapiv) (GLenum target, GLenum query, GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetMaterialfv) (GLenum face, GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetMaterialiv) (GLenum face, GLenum pname, GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetPixelMapfv) (GLenum map, GLfloat *values) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetPixelMapuiv) (GLenum map, GLuint *values) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetPixelMapusv) (GLenum map, GLushort *values) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetPointerv) (GLenum pname, GLvoid* *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetPolygonStipple) (GLubyte *mask) GLDRIVER_CALLSPEC_SUFFIX;
extern const GLubyte * GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetString) (GLenum name) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexEnvfv) (GLenum target, GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexEnviv) (GLenum target, GLenum pname, GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexGendv) (GLenum coord, GLenum pname, GLdouble *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexGenfv) (GLenum coord, GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexGeniv) (GLenum coord, GLenum pname, GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexImage) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexLevelParameterfv) (GLenum target, GLint level, GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexLevelParameteriv) (GLenum target, GLint level, GLenum pname, GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexParameterfv) (GLenum target, GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetTexParameteriv) (GLenum target, GLenum pname, GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glHint) (GLenum target, GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexMask) (GLuint mask) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexPointer) (GLenum type, GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexd) (GLdouble c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexdv) (const GLdouble *c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexf) (GLfloat c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexfv) (const GLfloat *c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexi) (GLint c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexiv) (const GLint *c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexs) (GLshort c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexsv) (const GLshort *c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexub) (GLubyte c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIndexubv) (const GLubyte *c) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glInitNames) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glInterleavedArrays) (GLenum format, GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern GLboolean GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIsEnabled) (GLenum cap) GLDRIVER_CALLSPEC_SUFFIX;
extern GLboolean GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIsList) (GLuint list) GLDRIVER_CALLSPEC_SUFFIX;
extern GLboolean GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIsTexture) (GLuint texture) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLightModelf) (GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLightModelfv) (GLenum pname, const GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLightModeli) (GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLightModeliv) (GLenum pname, const GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLightf) (GLenum light, GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLightfv) (GLenum light, GLenum pname, const GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLighti) (GLenum light, GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLightiv) (GLenum light, GLenum pname, const GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLineStipple) (GLint factor, GLushort pattern) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLineWidth) (GLfloat width) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glListBase) (GLuint base) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLoadIdentity) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLoadMatrixd) (const GLdouble *m) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLoadMatrixf) (const GLfloat *m) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLoadName) (GLuint name) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLogicOp) (GLenum opcode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMap1d) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMap1f) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMap2d) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMap2f) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMapGrid1d) (GLint un, GLdouble u1, GLdouble u2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMapGrid1f) (GLint un, GLfloat u1, GLfloat u2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMapGrid2d) (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMapGrid2f) (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMaterialf) (GLenum face, GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMaterialfv) (GLenum face, GLenum pname, const GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMateriali) (GLenum face, GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMaterialiv) (GLenum face, GLenum pname, const GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMatrixMode) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultMatrixd) (const GLdouble *m) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultMatrixf) (const GLfloat *m) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNewList) (GLuint list, GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3b) (GLbyte nx, GLbyte ny, GLbyte nz) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3bv) (const GLbyte *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3d) (GLdouble nx, GLdouble ny, GLdouble nz) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3f) (GLfloat nx, GLfloat ny, GLfloat nz) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3i) (GLint nx, GLint ny, GLint nz) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3s) (GLshort nx, GLshort ny, GLshort nz) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormal3sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glNormalPointer) (GLenum type, GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPassThrough) (GLfloat token) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelMapfv) (GLenum map, GLsizei mapsize, const GLfloat *values) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelMapuiv) (GLenum map, GLsizei mapsize, const GLuint *values) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelMapusv) (GLenum map, GLsizei mapsize, const GLushort *values) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelStoref) (GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelStorei) (GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelTransferf) (GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelTransferi) (GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPixelZoom) (GLfloat xfactor, GLfloat yfactor) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPointSize) (GLfloat size) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPolygonMode) (GLenum face, GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPolygonOffset) (GLfloat factor, GLfloat units) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPolygonStipple) (const GLubyte *mask) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPopAttrib) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPopClientAttrib) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPopMatrix) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPopName) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPrioritizeTextures) (GLsizei n, const GLuint *textures, const GLclampf *priorities) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPushAttrib) (GLbitfield mask) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPushClientAttrib) (GLbitfield mask) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPushMatrix) (void) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPushName) (GLuint name) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2d) (GLdouble x, GLdouble y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2f) (GLfloat x, GLfloat y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2i) (GLint x, GLint y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2s) (GLshort x, GLshort y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos2sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3d) (GLdouble x, GLdouble y, GLdouble z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3f) (GLfloat x, GLfloat y, GLfloat z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3i) (GLint x, GLint y, GLint z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3s) (GLshort x, GLshort y, GLshort z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos3sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4i) (GLint x, GLint y, GLint z, GLint w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4s) (GLshort x, GLshort y, GLshort z, GLshort w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRasterPos4sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glReadBuffer) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRectd) (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRectdv) (const GLdouble *v1, const GLdouble *v2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRectf) (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRectfv) (const GLfloat *v1, const GLfloat *v2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRecti) (GLint x1, GLint y1, GLint x2, GLint y2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRectiv) (const GLint *v1, const GLint *v2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRects) (GLshort x1, GLshort y1, GLshort x2, GLshort y2) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRectsv) (const GLshort *v1, const GLshort *v2) GLDRIVER_CALLSPEC_SUFFIX;
extern GLint GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRenderMode) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRotated) (GLdouble angle, GLdouble x, GLdouble y, GLdouble z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glScaled) (GLdouble x, GLdouble y, GLdouble z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glScalef) (GLfloat x, GLfloat y, GLfloat z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glScissor) (GLint x, GLint y, GLsizei width, GLsizei height) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSelectBuffer) (GLsizei size, GLuint *buffer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glShadeModel) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glStencilFunc) (GLenum func, GLint ref, GLuint mask) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glStencilMask) (GLuint mask) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1d) (GLdouble s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1f) (GLfloat s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1i) (GLint s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1s) (GLshort s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord1sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2d) (GLdouble s, GLdouble t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2f) (GLfloat s, GLfloat t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2i) (GLint s, GLint t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2s) (GLshort s, GLshort t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord2sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3d) (GLdouble s, GLdouble t, GLdouble r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3f) (GLfloat s, GLfloat t, GLfloat r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3i) (GLint s, GLint t, GLint r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3s) (GLshort s, GLshort t, GLshort r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord3sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4d) (GLdouble s, GLdouble t, GLdouble r, GLdouble q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4f) (GLfloat s, GLfloat t, GLfloat r, GLfloat q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4i) (GLint s, GLint t, GLint r, GLint q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4s) (GLshort s, GLshort t, GLshort r, GLshort q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoord4sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexEnvf) (GLenum target, GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexEnvfv) (GLenum target, GLenum pname, const GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexEnvi) (GLenum target, GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexEnviv) (GLenum target, GLenum pname, const GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexGend) (GLenum coord, GLenum pname, GLdouble param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexGendv) (GLenum coord, GLenum pname, const GLdouble *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexGenf) (GLenum coord, GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexGenfv) (GLenum coord, GLenum pname, const GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexGeni) (GLenum coord, GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexGeniv) (GLenum coord, GLenum pname, const GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexImage1D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexParameterf) (GLenum target, GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexParameterfv) (GLenum target, GLenum pname, const GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexParameteri) (GLenum target, GLenum pname, GLint param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexParameteriv) (GLenum target, GLenum pname, const GLint *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTranslated) (GLdouble x, GLdouble y, GLdouble z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTranslatef) (GLfloat x, GLfloat y, GLfloat z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2d) (GLdouble x, GLdouble y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2f) (GLfloat x, GLfloat y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2i) (GLint x, GLint y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2s) (GLshort x, GLshort y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex2sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3d) (GLdouble x, GLdouble y, GLdouble z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3f) (GLfloat x, GLfloat y, GLfloat z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3i) (GLint x, GLint y, GLint z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3s) (GLshort x, GLshort y, GLshort z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex3sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4i) (GLint x, GLint y, GLint z, GLint w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4s) (GLshort x, GLshort y, GLshort z, GLshort w) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertex4sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glViewport) (GLint x, GLint y, GLsizei width, GLsizei height) GLDRIVER_CALLSPEC_SUFFIX;

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.2 function pointers.
 */
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCopyTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDrawRangeElements) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexImage3D) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels) GLDRIVER_CALLSPEC_SUFFIX;

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.3 function pointers.
 */
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glActiveTexture) (GLenum texture) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glClientActiveTexture) (GLenum texture) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCompressedTexImage1D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCompressedTexImage3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCompressedTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glCompressedTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetCompressedTexImage) (GLenum target, GLint lod, GLvoid *img) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLoadTransposeMatrixd) (const GLdouble m[16]) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glLoadTransposeMatrixf) (const GLfloat m[16]) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultTransposeMatrixd) (const GLdouble m[16]) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultTransposeMatrixf) (const GLfloat m[16]) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1d) (GLenum target, GLdouble s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1dv) (GLenum target, const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1f) (GLenum target, GLfloat s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1fv) (GLenum target, const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1i) (GLenum target, GLint s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1iv) (GLenum target, const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1s) (GLenum target, GLshort s) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord1sv) (GLenum target, const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2d) (GLenum target, GLdouble s, GLdouble t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2dv) (GLenum target, const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2f) (GLenum target, GLfloat s, GLfloat t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2fv) (GLenum target, const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2i) (GLenum target, GLint s, GLint t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2iv) (GLenum target, const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2s) (GLenum target, GLshort s, GLshort t) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord2sv) (GLenum target, const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3d) (GLenum target, GLdouble s, GLdouble t, GLdouble r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3dv) (GLenum target, const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3f) (GLenum target, GLfloat s, GLfloat t, GLfloat r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3fv) (GLenum target, const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3i) (GLenum target, GLint s, GLint t, GLint r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3iv) (GLenum target, const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3s) (GLenum target, GLshort s, GLshort t, GLshort r) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord3sv) (GLenum target, const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4d) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4dv) (GLenum target, const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4f) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4fv) (GLenum target, const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4i) (GLenum target, GLint s, GLint t, GLint r, GLint q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4iv) (GLenum target, const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4s) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiTexCoord4sv) (GLenum target, const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSampleCoverage) (GLclampf value, GLboolean invert) GLDRIVER_CALLSPEC_SUFFIX;

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.4 function pointers.
 */
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBlendColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBlendEquation) (GLenum mode) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBlendFuncSeparate) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogCoordPointer) (GLenum type, GLsizei stride, const GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogCoordd) (GLdouble coord) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogCoorddv) (const GLdouble *coord) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogCoordf) (GLfloat coord) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glFogCoordfv) (const GLfloat *coord) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiDrawArrays) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMultiDrawElements) (GLenum mode, GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPointParameterf) (GLenum pname, GLfloat param) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glPointParameterfv) (GLenum pname, GLfloat *params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3b) (GLbyte red, GLbyte green, GLbyte blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3bv) (const GLbyte *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3d) (GLdouble red, GLdouble green, GLdouble blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3dv) (const GLdouble *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3f) (GLfloat red, GLfloat green, GLfloat blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3fv) (const GLfloat *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3i) (GLint red, GLint green, GLint blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3iv) (const GLint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3s) (GLshort red, GLshort green, GLshort blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3sv) (const GLshort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3ub) (GLubyte red, GLubyte green, GLubyte blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3ubv) (const GLubyte *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3ui) (GLuint red, GLuint green, GLuint blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3uiv) (const GLuint *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3us) (GLushort red, GLushort green, GLushort blue) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColor3usv) (const GLushort *v) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glSecondaryColorPointer) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2d) (GLdouble x, GLdouble y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2dv) (const GLdouble *p) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2f) (GLfloat x, GLfloat y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2fv) (const GLfloat *p) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2i) (GLint x, GLint y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2iv) (const GLint *p) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2s) (GLshort x, GLshort y) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos2sv) (const GLshort *p) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3d) (GLdouble x, GLdouble y, GLdouble z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3dv) (const GLdouble *p) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3f) (GLfloat x, GLfloat y, GLfloat z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3fv) (const GLfloat *p) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3i) (GLint x, GLint y, GLint z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3iv) (const GLint *p) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3s) (GLshort x, GLshort y, GLshort z) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glWindowPos3sv) (const GLshort *p) GLDRIVER_CALLSPEC_SUFFIX;

/*--------------------------------------------------------------------------*/

/*
 *      OpenGL version 1.5 function pointers.
 */
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBeginQuery) (GLenum target, GLuint id) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBindBuffer) (GLenum target, GLuint buffer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBufferData) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDeleteBuffers) (GLsizei n, const GLuint* buffers) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glDeleteQueries) (GLsizei n, const GLuint* ids) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glEndQuery) (GLenum target) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGenBuffers) (GLsizei n, GLuint* buffers) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGenQueries) (GLsizei n, GLuint* ids) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetBufferParameteriv) (GLenum target, GLenum pname, GLint* params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetBufferPointerv) (GLenum target, GLenum pname, GLvoid** params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetQueryObjectiv) (GLuint id, GLenum pname, GLint* params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetQueryObjectuiv) (GLuint id, GLenum pname, GLuint* params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glGetQueryiv) (GLenum target, GLenum pname, GLint params) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIsBuffer) (GLuint buffer) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glIsQuery) (GLuint id) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glMapBuffer) (GLenum target, GLenum access) GLDRIVER_CALLSPEC_SUFFIX;
extern void GLDRIVER_EXPORT_C GLDRIVER_CALLSPEC_PREFIX (*glUnmapBuffer) (GLenum target) GLDRIVER_CALLSPEC_SUFFIX;

/*--------------------------------------------------------------------------*/

#endif /* GLDRIVER_DYNAMIC_OPENGL_LINKING ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangling
 */
#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*/

/*
 *      Debug versions of the 1.1 OpenGL functions with tracing support
 */
#ifdef GLDRIVER_DEBUG_MODE
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glAccumDEBUG(GLenum op, GLfloat value, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glAlphaFuncDEBUG(GLenum func, GLclampf ref, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C GLboolean GLDRIVER_CALLSPEC_PREFIX glAreTexturesResidentDEBUG(GLsizei n, const GLuint *textures, GLboolean *residences, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glArrayElementDEBUG(GLint i, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glBeginDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glBindTextureDEBUG(GLenum target, GLuint texture, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glBitmapDEBUG(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glBlendFuncDEBUG(GLenum sfactor, GLenum dfactor, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCallListDEBUG(GLuint list, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCallListsDEBUG(GLsizei n, GLenum type, const GLvoid *lists, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glClearDEBUG(GLbitfield mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glClearAccumDEBUG(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glClearColorDEBUG(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glClearDepthDEBUG(GLclampd depth, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glClearIndexDEBUG(GLfloat c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glClearStencilDEBUG(GLint s, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glClipPlaneDEBUG(GLenum plane, const GLdouble *equation, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3bDEBUG(GLbyte red, GLbyte green, GLbyte blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3bvDEBUG(const GLbyte *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3dDEBUG(GLdouble red, GLdouble green, GLdouble blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3fDEBUG(GLfloat red, GLfloat green, GLfloat blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3iDEBUG(GLint red, GLint green, GLint blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3sDEBUG(GLshort red, GLshort green, GLshort blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3ubDEBUG(GLubyte red, GLubyte green, GLubyte blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3ubvDEBUG(const GLubyte *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3uiDEBUG(GLuint red, GLuint green, GLuint blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3uivDEBUG(const GLuint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3usDEBUG(GLushort red, GLushort green, GLushort blue, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor3usvDEBUG(const GLushort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4bDEBUG(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4bvDEBUG(const GLbyte *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4dDEBUG(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4fDEBUG(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4iDEBUG(GLint red, GLint green, GLint blue, GLint alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4sDEBUG(GLshort red, GLshort green, GLshort blue, GLshort alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4ubDEBUG(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4ubvDEBUG(const GLubyte *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4uiDEBUG(GLuint red, GLuint green, GLuint blue, GLuint alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4uivDEBUG(const GLuint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4usDEBUG(GLushort red, GLushort green, GLushort blue, GLushort alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColor4usvDEBUG(const GLushort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColorMaskDEBUG(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColorMaterialDEBUG(GLenum face, GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glColorPointerDEBUG(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCopyPixelsDEBUG(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCopyTexImage1DDEBUG(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCopyTexImage2DDEBUG(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCopyTexSubImage1DDEBUG(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCopyTexSubImage2DDEBUG(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glCullFaceDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDeleteListsDEBUG(GLuint list, GLsizei range, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDeleteTexturesDEBUG(GLsizei n, const GLuint *textures, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDepthFuncDEBUG(GLenum func, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDepthMaskDEBUG(GLboolean flag, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDepthRangeDEBUG(GLclampd zNear, GLclampd zFar, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDisableDEBUG(GLenum cap, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDisableClientStateDEBUG(GLenum array, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDrawArraysDEBUG(GLenum mode, GLint first, GLsizei count, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDrawBufferDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDrawElementsDEBUG(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glDrawPixelsDEBUG(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEdgeFlagDEBUG(GLboolean flag, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEdgeFlagPointerDEBUG(GLsizei stride, const GLvoid *pointer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEdgeFlagvDEBUG(const GLboolean *flag, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEnableDEBUG(GLenum cap, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEnableClientStateDEBUG(GLenum array, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEndDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEndListDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord1dDEBUG(GLdouble u, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord1dvDEBUG(const GLdouble *u, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord1fDEBUG(GLfloat u, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord1fvDEBUG(const GLfloat *u, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord2dDEBUG(GLdouble u, GLdouble v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord2dvDEBUG(const GLdouble *u, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord2fDEBUG(GLfloat u, GLfloat v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalCoord2fvDEBUG(const GLfloat *u, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalMesh1DEBUG(GLenum mode, GLint i1, GLint i2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalMesh2DEBUG(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalPoint1DEBUG(GLint i, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glEvalPoint2DEBUG(GLint i, GLint j, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFeedbackBufferDEBUG(GLsizei size, GLenum type, GLfloat *buffer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFinishDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFlushDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFogfDEBUG(GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFogfvDEBUG(GLenum pname, const GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFogiDEBUG(GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFogivDEBUG(GLenum pname, const GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFrontFaceDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glFrustumDEBUG(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C GLuint GLDRIVER_CALLSPEC_PREFIX glGenListsDEBUG(GLsizei range, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGenTexturesDEBUG(GLsizei n, GLuint *textures, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetBooleanvDEBUG(GLenum pname, GLboolean *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetClipPlaneDEBUG(GLenum plane, GLdouble *equation, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetDoublevDEBUG(GLenum pname, GLdouble *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C GLenum GLDRIVER_CALLSPEC_PREFIX glGetErrorDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetFloatvDEBUG(GLenum pname, GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetIntegervDEBUG(GLenum pname, GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetLightfvDEBUG(GLenum light, GLenum pname, GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetLightivDEBUG(GLenum light, GLenum pname, GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetMapdvDEBUG(GLenum target, GLenum query, GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetMapfvDEBUG(GLenum target, GLenum query, GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetMapivDEBUG(GLenum target, GLenum query, GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetMaterialfvDEBUG(GLenum face, GLenum pname, GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetMaterialivDEBUG(GLenum face, GLenum pname, GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetPixelMapfvDEBUG(GLenum map, GLfloat *values, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetPixelMapuivDEBUG(GLenum map, GLuint *values, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetPixelMapusvDEBUG(GLenum map, GLushort *values, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetPointervDEBUG(GLenum pname, GLvoid* *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetPolygonStippleDEBUG(GLubyte *mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C const GLubyte* GLDRIVER_CALLSPEC_PREFIX glGetStringDEBUG(GLenum name, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexEnvfvDEBUG(GLenum target, GLenum pname, GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexEnvivDEBUG(GLenum target, GLenum pname, GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexGendvDEBUG(GLenum coord, GLenum pname, GLdouble *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexGenfvDEBUG(GLenum coord, GLenum pname, GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexGenivDEBUG(GLenum coord, GLenum pname, GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexImageDEBUG(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexLevelParameterfvDEBUG(GLenum target, GLint level, GLenum pname, GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexLevelParameterivDEBUG(GLenum target, GLint level, GLenum pname, GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexParameterfvDEBUG(GLenum target, GLenum pname, GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glGetTexParameterivDEBUG(GLenum target, GLenum pname, GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glHintDEBUG(GLenum target, GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexMaskDEBUG(GLuint mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexPointerDEBUG(GLenum type, GLsizei stride, const GLvoid *pointer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexdDEBUG(GLdouble c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexdvDEBUG(const GLdouble *c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexfDEBUG(GLfloat c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexfvDEBUG(const GLfloat *c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexiDEBUG(GLint c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexivDEBUG(const GLint *c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexsDEBUG(GLshort c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexsvDEBUG(const GLshort *c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexubDEBUG(GLubyte c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glIndexubvDEBUG(const GLubyte *c, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glInitNamesDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glInterleavedArraysDEBUG(GLenum format, GLsizei stride, const GLvoid *pointer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C GLboolean GLDRIVER_CALLSPEC_PREFIX glIsEnabledDEBUG(GLenum cap, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C GLboolean GLDRIVER_CALLSPEC_PREFIX glIsListDEBUG(GLuint list, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C GLboolean GLDRIVER_CALLSPEC_PREFIX glIsTextureDEBUG(GLuint texture, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightModelfDEBUG(GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightModelfvDEBUG(GLenum pname, const GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightModeliDEBUG(GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightModelivDEBUG(GLenum pname, const GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightfDEBUG(GLenum light, GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightfvDEBUG(GLenum light, GLenum pname, const GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightiDEBUG(GLenum light, GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLightivDEBUG(GLenum light, GLenum pname, const GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLineStippleDEBUG(GLint factor, GLushort pattern, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLineWidthDEBUG(GLfloat width, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glListBaseDEBUG(GLuint base, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLoadIdentityDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLoadMatrixdDEBUG(const GLdouble *m, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLoadMatrixfDEBUG(const GLfloat *m, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLoadNameDEBUG(GLuint name, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glLogicOpDEBUG(GLenum opcode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMap1dDEBUG(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMap1fDEBUG(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMap2dDEBUG(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMap2fDEBUG(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMapGrid1dDEBUG(GLint un, GLdouble u1, GLdouble u2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMapGrid1fDEBUG(GLint un, GLfloat u1, GLfloat u2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMapGrid2dDEBUG(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMapGrid2fDEBUG(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMaterialfDEBUG(GLenum face, GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMaterialfvDEBUG(GLenum face, GLenum pname, const GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMaterialiDEBUG(GLenum face, GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMaterialivDEBUG(GLenum face, GLenum pname, const GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMatrixModeDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMultMatrixdDEBUG(const GLdouble *m, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glMultMatrixfDEBUG(const GLfloat *m, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNewListDEBUG(GLuint list, GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3bDEBUG(GLbyte nx, GLbyte ny, GLbyte nz, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3bvDEBUG(const GLbyte *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3dDEBUG(GLdouble nx, GLdouble ny, GLdouble nz, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3fDEBUG(GLfloat nx, GLfloat ny, GLfloat nz, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3iDEBUG(GLint nx, GLint ny, GLint nz, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3sDEBUG(GLshort nx, GLshort ny, GLshort nz, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormal3svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glNormalPointerDEBUG(GLenum type, GLsizei stride, const GLvoid *pointer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glOrthoDEBUG(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPassThroughDEBUG(GLfloat token, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelMapfvDEBUG(GLenum map, GLsizei mapsize, const GLfloat *values, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelMapuivDEBUG(GLenum map, GLsizei mapsize, const GLuint *values, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelMapusvDEBUG(GLenum map, GLsizei mapsize, const GLushort *values, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelStorefDEBUG(GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelStoreiDEBUG(GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelTransferfDEBUG(GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelTransferiDEBUG(GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPixelZoomDEBUG(GLfloat xfactor, GLfloat yfactor, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPointSizeDEBUG(GLfloat size, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPolygonModeDEBUG(GLenum face, GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPolygonOffsetDEBUG(GLfloat factor, GLfloat units, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPolygonStippleDEBUG(const GLubyte *mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPopAttribDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPopClientAttribDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPopMatrixDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPopNameDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPrioritizeTexturesDEBUG(GLsizei n, const GLuint *textures, const GLclampf *priorities, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPushAttribDEBUG(GLbitfield mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPushClientAttribDEBUG(GLbitfield mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPushMatrixDEBUG( const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glPushNameDEBUG(GLuint name, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2dDEBUG(GLdouble x, GLdouble y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2fDEBUG(GLfloat x, GLfloat y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2iDEBUG(GLint x, GLint y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2sDEBUG(GLshort x, GLshort y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos2svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3dDEBUG(GLdouble x, GLdouble y, GLdouble z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3fDEBUG(GLfloat x, GLfloat y, GLfloat z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3iDEBUG(GLint x, GLint y, GLint z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3sDEBUG(GLshort x, GLshort y, GLshort z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos3svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4dDEBUG(GLdouble x, GLdouble y, GLdouble z, GLdouble w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4fDEBUG(GLfloat x, GLfloat y, GLfloat z, GLfloat w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4iDEBUG(GLint x, GLint y, GLint z, GLint w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4sDEBUG(GLshort x, GLshort y, GLshort z, GLshort w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRasterPos4svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glReadBufferDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glReadPixelsDEBUG(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectdDEBUG(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectdvDEBUG(const GLdouble *v1, const GLdouble *v2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectfDEBUG(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectfvDEBUG(const GLfloat *v1, const GLfloat *v2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectiDEBUG(GLint x1, GLint y1, GLint x2, GLint y2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectivDEBUG(const GLint *v1, const GLint *v2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectsDEBUG(GLshort x1, GLshort y1, GLshort x2, GLshort y2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRectsvDEBUG(const GLshort *v1, const GLshort *v2, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C GLint GLDRIVER_CALLSPEC_PREFIX glRenderModeDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRotatedDEBUG(GLdouble angle, GLdouble x, GLdouble y, GLdouble z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glRotatefDEBUG(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glScaledDEBUG(GLdouble x, GLdouble y, GLdouble z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glScalefDEBUG(GLfloat x, GLfloat y, GLfloat z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glScissorDEBUG(GLint x, GLint y, GLsizei width, GLsizei height, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glSelectBufferDEBUG(GLsizei size, GLuint *buffer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glShadeModelDEBUG(GLenum mode, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glStencilFuncDEBUG(GLenum func, GLint ref, GLuint mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glStencilMaskDEBUG(GLuint mask, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glStencilOpDEBUG(GLenum fail, GLenum zfail, GLenum zpass, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1dDEBUG(GLdouble s, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1fDEBUG(GLfloat s, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1iDEBUG(GLint s, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1sDEBUG(GLshort s, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord1svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2dDEBUG(GLdouble s, GLdouble t, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2fDEBUG(GLfloat s, GLfloat t, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2iDEBUG(GLint s, GLint t, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2sDEBUG(GLshort s, GLshort t, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord2svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3dDEBUG(GLdouble s, GLdouble t, GLdouble r, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3fDEBUG(GLfloat s, GLfloat t, GLfloat r, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3iDEBUG(GLint s, GLint t, GLint r, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3sDEBUG(GLshort s, GLshort t, GLshort r, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord3svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4dDEBUG(GLdouble s, GLdouble t, GLdouble r, GLdouble q, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4fDEBUG(GLfloat s, GLfloat t, GLfloat r, GLfloat q, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4iDEBUG(GLint s, GLint t, GLint r, GLint q, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4sDEBUG(GLshort s, GLshort t, GLshort r, GLshort q, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoord4svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexCoordPointerDEBUG(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexEnvfDEBUG(GLenum target, GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexEnvfvDEBUG(GLenum target, GLenum pname, const GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexEnviDEBUG(GLenum target, GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexEnvivDEBUG(GLenum target, GLenum pname, const GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexGendDEBUG(GLenum coord, GLenum pname, GLdouble param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexGendvDEBUG(GLenum coord, GLenum pname, const GLdouble *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexGenfDEBUG(GLenum coord, GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexGenfvDEBUG(GLenum coord, GLenum pname, const GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexGeniDEBUG(GLenum coord, GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexGenivDEBUG(GLenum coord, GLenum pname, const GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexImage1DDEBUG(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexImage2DDEBUG(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexParameterfDEBUG(GLenum target, GLenum pname, GLfloat param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexParameterfvDEBUG(GLenum target, GLenum pname, const GLfloat *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexParameteriDEBUG(GLenum target, GLenum pname, GLint param, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexParameterivDEBUG(GLenum target, GLenum pname, const GLint *params, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexSubImage1DDEBUG(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTexSubImage2DDEBUG(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTranslatedDEBUG(GLdouble x, GLdouble y, GLdouble z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glTranslatefDEBUG(GLfloat x, GLfloat y, GLfloat z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2dDEBUG(GLdouble x, GLdouble y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2fDEBUG(GLfloat x, GLfloat y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2iDEBUG(GLint x, GLint y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2sDEBUG(GLshort x, GLshort y, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex2svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3dDEBUG(GLdouble x, GLdouble y, GLdouble z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3fDEBUG(GLfloat x, GLfloat y, GLfloat z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3iDEBUG(GLint x, GLint y, GLint z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3sDEBUG(GLshort x, GLshort y, GLshort z, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex3svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4dDEBUG(GLdouble x, GLdouble y, GLdouble z, GLdouble w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4dvDEBUG(const GLdouble *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4fDEBUG(GLfloat x, GLfloat y, GLfloat z, GLfloat w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4fvDEBUG(const GLfloat *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4iDEBUG(GLint x, GLint y, GLint z, GLint w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4ivDEBUG(const GLint *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4sDEBUG(GLshort x, GLshort y, GLshort z, GLshort w, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertex4svDEBUG(const GLshort *v, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glVertexPointerDEBUG(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
GLDRIVER_DEBUG_EXPORT_C void GLDRIVER_CALLSPEC_PREFIX glViewportDEBUG(GLint x, GLint y, GLsizei width, GLsizei height, const char* file, UInt32 line ) GLDRIVER_CALLSPEC_SUFFIX;
#endif /* GLDRIVER_DEBUG_MODE ? */

/*--------------------------------------------------------------------------*/

/*
 *      Debug versions of the 1.5 OpenGL functions with tracing support
 */
#ifdef GLDRIVER_DEBUG_MODE
GLDRIVER_DEBUG_EXPORT_C void glBeginQueryDEBUG( GLenum target, GLuint id, const char* file, UInt32 line );
GLDRIVER_DEBUG_EXPORT_C void glBindBufferDEBUG( GLenum target, GLuint buffer, const char* file, UInt32 line );
#endif /* GLDRIVER_DEBUG_MODE ? */

/*-------------------------------------------------------------------------*/

/*
 *      End the C++ name mangling protection
 */
#ifdef __cplusplus
};
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros that redirect OpenGL 1.1 function calls to the debug versions of
 *      the functions with added trace information.
 */
#ifdef GLDRIVER_DEBUG_MODE
#define glAccum( op, value ) glAccumDEBUG( op, value, __FILE__, __LINE__ )
#define glAlphaFunc( func, ref ) glAlphaFuncDEBUG( func, ref, __FILE__, __LINE__ )
#define glAreTexturesResident( n, textures, residences ) glAreTexturesResidentDEBUG( n, textures, residences, __FILE__, __LINE__ )
#define glArrayElement( i ) glArrayElementDEBUG( i, __FILE__, __LINE__ )
#define glBegin( mode ) glBeginDEBUG( mode, __FILE__, __LINE__ )
#define glBindTexture( target, texture ) glBindTextureDEBUG( target, texture, __FILE__, __LINE__ )
#define glBitmap( width, height, xorig, yorig, xmove, ymove, bitmap ) glBitmapDEBUG( width, height, xorig, yorig, xmove, ymove, bitmap, __FILE__, __LINE__ )
#define glBlendFunc( sfactor, dfactor ) glBlendFuncDEBUG( sfactor, dfactor, __FILE__, __LINE__ )
#define glCallList( list ) glCallListDEBUG( list, __FILE__, __LINE__ )
#define glCallLists( n, type, lists ) glCallListsDEBUG( n, type, lists, __FILE__, __LINE__ )
#define glClear( mask ) glClearDEBUG( mask, __FILE__, __LINE__ )
#define glClearAccum( red, green, blue, alpha ) glClearAccumDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glClearColor( red, green, blue, alpha ) glClearColorDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glClearDepth( depth ) glClearDepthDEBUG( depth, __FILE__, __LINE__ )
#define glClearIndex( c ) glClearIndexDEBUG( c, __FILE__, __LINE__ )
#define glClearStencil( s ) glClearStencilDEBUG( s, __FILE__, __LINE__ )
#define glClipPlane( plane, equation ) glClipPlaneDEBUG( plane, equation, __FILE__, __LINE__ )
#define glColor3b( red, green, blue) glColor3bDEBUG( red, green, blue, __FILE__, __LINE__ )
#define glColor3bv( v ) glColor3bvDEBUG( v, __FILE__, __LINE__ )
#define glColor3d( red, green, blue ) glColor3dDEBUG( red, green, blue, __FILE__, __LINE__ )
#define glColor3dv( v ) glColor3dvDEBUG( v, __FILE__, __LINE__ )
#define glColor3f( red, green, blue ) glColor3fDEBUG( red, green, blue, __FILE__, __LINE__ )
#define glColor3fv( v ) glColor3fvDEBUG( v, __FILE__, __LINE__ )
#define glColor3i( red, green, blue ) glColor3iDEBUG( red, green, blue, __FILE__, __LINE__ )
#define glColor3iv(  v ) glColor3ivDEBUG( v, __FILE__, __LINE__ )
#define glColor3s( red, green, blue ) glColor3sDEBUG( red, green, blue, __FILE__, __LINE )
#define glColor3sv( v ) glColor3svDEBUG( v, __FILE__, __LINE__ )
#define glColor3ub( red, green, blue ) glColor3ubDEBUG( red, green, blue, __FILE__, __LINE__ )
#define glColor3ubv( v ) glColor3ubvDEBUG( v, __FILE__, __LINE__ )
#define glColor3ui( red, green, blue ) glColor3ui( red, green, blue, __FILE__, __LINE__ )
#define glColor3uiv( v ) glColor3uivDEBUG( v, __FILE__, __LINE__ )
#define glColor3us( red, green, blue ) glColor3usDEBUG( red, green, blue, __FILE__, __LINE__ )
#define glColor3usv( v ) glColor3usvDEBUG( v, __FILE__, __LINE__ )
#define glColor4b( red, green, blue, alpha ) glColor4bDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4bv( v ) glColor4bvDEBUG( v, __FILE__, __LINE__ )
#define glColor4d( red, green, blue, alpha ) glColor4dDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4dv( v ) glColor4dvDEBUG( v, __FILE__, __LINE__ )
#define glColor4f( red, green, blue, alpha ) glColor4fDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4fv( v ) glColor4fvDEBUG( v, __FILE__, __LINE__ )
#define glColor4i( red, green, blue, alpha ) glColor4iDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4iv( v ) glColor4ivDEBUG( v, __FILE__, __LINE__ )
#define glColor4s( red, green, blue, alpha ) glColor4sDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4sv( v ) glColor4svDEBUG( v, __FILE__, __LINE__ )
#define glColor4ub( red, green, blue, alpha ) glColor4ubDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4ubv( v ) glColor4ubvDEBUG( v, __FILE__, __LINE__ )
#define glColor4ui( red, green, blue, alpha ) glColor4uiDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4uiv( v ) glColor4uivDEBUG( v, __FILE__, __LINE__ )
#define glColor4us( red, green, blue, alpha ) glColor4usDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColor4usv( v ) glColor4usvDEBUG( v, __FILE__, __LINE__ )
#define glColorMask( red, green, blue, alpha ) glColorMaskDEBUG( red, green, blue, alpha, __FILE__, __LINE__ )
#define glColorMaterial( face, mode ) glColorMaterialDEBUG( face, mode, __FILE__, __LINE__ )
#define glColorPointer( size, type, stride, pointer ) glColorPointerDEBUG( size, type, stride, pointer, __FILE__, __LINE__ )
#define glCopyPixels( x, y, width, height, type ) glCopyPixelsDEBUG( x, y, width, height, type, __FILE__, __LINE__ )
#define glCopyTexImage1D( target, level, internalFormat, x, y, width, border ) glCopyTexImage1DDEBUG( target, level, internalFormat, x, y, width, border, __FILE__, __LINE__ )
#define glCopyTexImage2D( target, level, internalFormat, x, y, width, height, border ) glCopyTexImage2DDEBUG( target, level, internalFormat, x, y, width, height, border, __FILE__, __LINE__ )
#define glCopyTexSubImage1D( target, level, xoffset, x, y, width ) glCopyTexSubImage1DDEBUG( target, level, xoffset, x, y, width, __FILE__, __LINE__ )
#define glCopyTexSubImage2D( target, level, xoffset, yoffset, x, y, width, height ) glCopyTexSubImage2DDEBUG( target, level, xoffset, yoffset, x, y, width, height, __FILE__, __LINE__ )
#define glCullFace( mode ) glCullFaceDEBUG( mode, __FILE__, __LINE__ )
#define glDeleteLists( list, range) glDeleteListsDEBUG( list, range, __FILE__, __LINE__ )
#define glDeleteTextures( n, textures) glDeleteTexturesDEBUG( n, textures, __FILE__, __LINE__ )
#define glDepthFunc( func) glDepthFuncDEBUG( func, __FILE__, __LINE__ )
#define glDepthMask( flag) glDepthMaskDEBUG( flag, __FILE__, __LINE__ )
#define glDepthRange( zNear, zFar) glDepthRangeDEBUG( zNear, zFar, __FILE__, __LINE__ )
#define glDisable( cap) glDisableDEBUG( cap, __FILE__, __LINE__ )
#define glDisableClientState( array) glDisableClientStateDEBUG( array, __FILE__, __LINE__ )
#define glDrawArrays( mode, first, count) glDrawArraysDEBUG( mode, first, count, __FILE__, __LINE__ )
#define glDrawBuffer( mode) glDrawBufferDEBUG( mode, __FILE__, __LINE__ )
#define glDrawElements( mode, count, type, indices) glDrawElementsDEBUG( mode, count, type, indices, __FILE__, __LINE__ )
#define glDrawPixels( width, height, format, type, pixels) glDrawPixelsDEBUG( width, height, format, type, pixels, __FILE__, __LINE__ )
#define glEdgeFlag( flag) glEdgeFlagDEBUG( flag, __FILE__, __LINE__ )
#define glEdgeFlagPointer( stride, pointer) glEdgeFlagPointerDEBUG( stride, pointer, __FILE__, __LINE__ )
#define glEdgeFlagv( flag) glEdgeFlagvDEBUG( flag, __FILE__, __LINE__ )
#define glEnable( cap) glEnableDEBUG( cap, __FILE__, __LINE__ )
#define glEnableClientState( array) glEnableClientStateDEBUG( array, __FILE__, __LINE__ )
#define glEnd() glEndDEBUG( __FILE__, __LINE__ )
#define glEndList() glEndListDEBUG( __FILE__, __LINE__ )
#define glEvalCoord1d( u) glEvalCoord1dDEBUG( u, __FILE__, __LINE__ )
#define glEvalCoord1dv( u) glEvalCoord1dvDEBUG( u, __FILE__, __LINE__ )
#define glEvalCoord1f( u) glEvalCoord1fDEBUG( u, __FILE__, __LINE__ )
#define glEvalCoord1fv( u) glEvalCoord1fvDEBUG( u, __FILE__, __LINE__ )
#define glEvalCoord2d( u, v) glEvalCoord2dDEBUG( u, v, __FILE__, __LINE__ )
#define glEvalCoord2dv( u) glEvalCoord2dvDEBUG( u, __FILE__, __LINE__ )
#define glEvalCoord2f( u, v) glEvalCoord2fDEBUG( u, v, __FILE__, __LINE__ )
#define glEvalCoord2fv( u) glEvalCoord2fvDEBUG( u, __FILE__, __LINE__ )
#define glEvalMesh1( mode, i1, i2) glEvalMesh1DEBUG( mode, i1, i2, __FILE__, __LINE__ )
#define glEvalMesh2( mode, i1, i2, j1, j2) glEvalMesh2DEBUG( mode, i1, i2, j1, j2, __FILE__, __LINE__ )
#define glEvalPoint1( i) glEvalPoint1DEBUG( i, __FILE__, __LINE__ )
#define glEvalPoint2( i, j) glEvalPoint2DEBUG( i, j, __FILE__, __LINE__ )
#define glFeedbackBuffer( size,  type, buffer) glFeedbackBufferDEBUG(size, type, *buffer, __FILE__, __LINE__ )
#define glFinish() glFinishDEBUG( __FILE__, __LINE__ )
#define glFlush() glFlushDEBUG( __FILE__, __LINE__ )
#define glFogf( pname, param) glFogfDEBUG( pname, param, __FILE__, __LINE__ )
#define glFogfv(pname, params) glFogfvDEBUG( pname, params, __FILE__, __LINE__ )
#define glFogi(pname, param) glFogiDEBUG(pname, param, __FILE__, __LINE__ )
#define glFogiv(pname, params) glFogivDEBUG(pname, params, __FILE__, __LINE__ )
#define glFrontFace( mode) glFrontFaceDEBUG( mode, __FILE__, __LINE__ )
#define glFrustum( left,  right, bottom, top, zNear, zFar) glFrustumDEBUG(left, right, bottom, top, zNear, zFar, __FILE__, __LINE__ )
#define glGenLists( range) glGenListsDEBUG( range, __FILE__, __LINE__ )
#define glGenTextures( n, textures) glGenTexturesDEBUG( n, textures, __FILE__, __LINE__ )
#define glGetBooleanv( pname, params) glGetBooleanvDEBUG( pname, params, __FILE__, __LINE__ )
#define glGetClipPlane( plane, equation) glGetClipPlaneDEBUG( plane, equation, __FILE__, __LINE__ )
#define glGetDoublev( pname, params) glGetDoublevDEBUG( pname, params, __FILE__, __LINE__ )
#define glGetError() glGetErrorDEBUG( __FILE__, __LINE__ )
#define glGetFloatv( pname, params) glGetFloatvDEBUG( pname, params, __FILE__, __LINE__ )
#define glGetIntegerv( pname, params) glGetIntegervDEBUG( pname, params, __FILE__, __LINE__ )
#define glGetLightfv( light, pname, params) glGetLightfvDEBUG( light, pname, params, __FILE__, __LINE__ )
#define glGetLightiv( light, pname, params) glGetLightivDEBUG( light, pname, params, __FILE__, __LINE__ )
#define glGetMapdv( target, query, v) glGetMapdvDEBUG(target, query, v, __FILE__, __LINE__ )
#define glGetMapfv( target, query, v) glGetMapfvDEBUG(target, query, v, __FILE__, __LINE__ )
#define glGetMapiv( target, query, v) glGetMapivDEBUG(target, query, v, __FILE__, __LINE__ )
#define glGetMaterialfv( face,  pname,  params) glGetMaterialfvDEBUG( face,  pname,  params, __FILE__, __LINE__ )
#define glGetMaterialiv( face,  pname,  params) glGetMaterialivDEBUG( face,  pname,  params, __FILE__, __LINE__ )
#define glGetPixelMapfv( map,  values) glGetPixelMapfvDEBUG( map,  values, __FILE__, __LINE__ )
#define glGetPixelMapuiv( map,  values) glGetPixelMapuivDEBUG( map,  values, __FILE__, __LINE__ )
#define glGetPixelMapusv( map,  values) glGetPixelMapusvDEBUG( map,  values, __FILE__, __LINE__ )
#define glGetPointerv( pname,  params) glGetPointervDEBUG( pname,  params, __FILE__, __LINE__ )
#define glGetPolygonStipple( mask) glGetPolygonStippleDEBUG( mask, __FILE__, __LINE__ )
#define glGetString( name) glGetStringDEBUG( name, __FILE__, __LINE__ )
#define glGetTexEnvfv( target,  pname,  params) glGetTexEnvfvDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glGetTexEnviv( target,  pname,  params) glGetTexEnvivDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glGetTexGendv( coord,  pname,  params) glGetTexGendvDEBUG( coord,  pname,  params, __FILE__, __LINE__ )
#define glGetTexGenfv( coord,  pname,  params) glGetTexGenfvDEBUG( coord,  pname,  params, __FILE__, __LINE__ )
#define glGetTexGeniv( coord,  pname,  params) glGetTexGenivDEBUG( coord,  pname,  params, __FILE__, __LINE__ )
#define glGetTexImage( target,  level,  format,  type,  pixels) glGetTexImageDEBUG( target,  level,  format,  type,  pixels, __FILE__, __LINE__ )
#define glGetTexLevelParameterfv( target,  level,  pname,  params) glGetTexLevelParameterfvDEBUG( target,  level,  pname,  params, __FILE__, __LINE__ )
#define glGetTexLevelParameteriv( target,  level,  pname,  params) glGetTexLevelParameterivDEBUG( target,  level,  pname,  params, __FILE__, __LINE__ )
#define glGetTexParameterfv( target,  pname,  params) glGetTexParameterfvDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glGetTexParameteriv( target,  pname,  params) glGetTexParameterivDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glHint( target,  mode) glHintDEBUG( target,  mode, __FILE__, __LINE__ )
#define glIndexMask( mask) glIndexMaskDEBUG( mask, __FILE__, __LINE__ )
#define glIndexPointer( type,  stride,  pointer) glIndexPointerDEBUG( type,  stride,  pointer, __FILE__, __LINE__ )
#define glIndexd( c) glIndexdDEBUG( c, __FILE__, __LINE__ )
#define glIndexdv( c) glIndexdvDEBUG( c, __FILE__, __LINE__ )
#define glIndexf( c) glIndexfDEBUG( c, __FILE__, __LINE__ )
#define glIndexfv( c) glIndexfvDEBUG( c, __FILE__, __LINE__ )
#define glIndexi( c) glIndexiDEBUG( c, __FILE__, __LINE__ )
#define glIndexiv( c) glIndexivDEBUG( c, __FILE__, __LINE__ )
#define glIndexs( c) glIndexsDEBUG( c, __FILE__, __LINE__ )
#define glIndexsv( c) glIndexsvDEBUG( c, __FILE__, __LINE__ )
#define glIndexub( c) glIndexubDEBUG( c, __FILE__, __LINE__ )
#define glIndexubv( c) glIndexubvDEBUG( c, __FILE__, __LINE__ )
#define glInitNames(void) glInitNamesDEBUG( __FILE__, __LINE__ )
#define glInterleavedArrays( format,  stride,  pointer) glInterleavedArraysDEBUG( format,  stride,  pointer, __FILE__, __LINE__ )
#define glIsEnabled( cap) glIsEnabledDEBUG( cap, __FILE__, __LINE__ )
#define glIsList( list) glIsListDEBUG( list, __FILE__, __LINE__ )
#define glIsTexture( texture) glIsTextureDEBUG( texture, __FILE__, __LINE__ )
#define glLightModelf( pname,  param) glLightModelfDEBUG( pname,  param, __FILE__, __LINE__ )
#define glLightModelfv( pname,  params) glLightModelfvDEBUG( pname,  params, __FILE__, __LINE__ )
#define glLightModeli( pname,  param) glLightModeliDEBUG( pname,  param, __FILE__, __LINE__ )
#define glLightModeliv( pname,  params) glLightModelivDEBUG( pname,  params, __FILE__, __LINE__ )
#define glLightf( light,  pname,  param) glLightfDEBUG( light,  pname,  param, __FILE__, __LINE__ )
#define glLightfv( light,  pname,  params) glLightfvDEBUG( light,  pname,  params, __FILE__, __LINE__ )
#define glLighti( light,  pname,  param) glLightiDEBUG( light,  pname,  param, __FILE__, __LINE__ )
#define glLightiv( light,  pname,  params) glLightivDEBUG( light,  pname,  params, __FILE__, __LINE__ )
#define glLineStipple( factor,  pattern) glLineStippleDEBUG( factor,  pattern, __FILE__, __LINE__ )
#define glLineWidth( width) glLineWidthDEBUG( width, __FILE__, __LINE__ )
#define glListBase( base) glListBaseDEBUG( base, __FILE__, __LINE__ )
#define glLoadIdentity() glLoadIdentityDEBUG( __FILE__, __LINE__ )
#define glLoadMatrixd( m) glLoadMatrixdDEBUG( m, __FILE__, __LINE__ )
#define glLoadMatrixf( m) glLoadMatrixfDEBUG( m, __FILE__, __LINE__ )
#define glLoadName( name) glLoadNameDEBUG( name, __FILE__, __LINE__ )
#define glLogicOp( opcode) glLogicOpDEBUG( opcode, __FILE__, __LINE__ )
#define glMap1d( target,  u1,  u2,  stride,  order,  points) glMap1dDEBUG( target,  u1,  u2,  stride,  order,  points, __FILE__, __LINE__ )
#define glMap1f( target,  u1,  u2,  stride,  order,  points) glMap1fDEBUG( target,  u1,  u2,  stride,  order,  points, __FILE__, __LINE__ )
#define glMap2d( target,  u1,  u2,  ustride,  uorder,  v1,  v2,  vstride,  vorder,  points) glMap2dDEBUG( target,  u1,  u2,  ustride,  uorder,  v1,  v2,  vstride,  vorder,  points, __FILE__, __LINE__ )
#define glMap2f( target,  u1,  u2,  ustride,  uorder,  v1,  v2,  vstride,  vorder,  points) glMap2fDEBUG( target,  u1,  u2,  ustride,  uorder,  v1,  v2,  vstride,  vorder,  points, __FILE__, __LINE__ )
#define glMapGrid1d( un,  u1,  u2) glMapGrid1dDEBUG( un,  u1,  u2, __FILE__, __LINE__ )
#define glMapGrid1f( un,  u1,  u2) glMapGrid1fDEBUG( un,  u1,  u2, __FILE__, __LINE__ )
#define glMapGrid2d( un,  u1,  u2,  vn,  v1,  v2) glMapGrid2dDEBUG( un,  u1,  u2,  vn,  v1,  v2, __FILE__, __LINE__ )
#define glMapGrid2f( un,  u1,  u2,  vn,  v1,  v2) glMapGrid2fDEBUG( un,  u1,  u2,  vn,  v1,  v2, __FILE__, __LINE__ )
#define glMaterialf( face,  pname,  param) glMaterialfDEBUG( face,  pname,  param, __FILE__, __LINE__ )
#define glMaterialfv( face,  pname,  params) glMaterialfvDEBUG( face,  pname,  params, __FILE__, __LINE__ )
#define glMateriali( face,  pname,  param) glMaterialiDEBUG( face,  pname,  param, __FILE__, __LINE__ )
#define glMaterialiv( face,  pname,  params) glMaterialivDEBUG( face,  pname,  params, __FILE__, __LINE__ )
#define glMatrixMode( mode) glMatrixModeDEBUG( mode, __FILE__, __LINE__ )
#define glMultMatrixd( m) glMultMatrixdDEBUG( m, __FILE__, __LINE__ )
#define glMultMatrixf( m) glMultMatrixfDEBUG( m, __FILE__, __LINE__ )
#define glNewList( list,  mode) glNewListDEBUG( list,  mode, __FILE__, __LINE__ )
#define glNormal3b( nx,  ny,  nz) glNormal3bDEBUG( nx,  ny,  nz, __FILE__, __LINE__ )
#define glNormal3bv( v) glNormal3bvDEBUG( v, __FILE__, __LINE__ )
#define glNormal3d( nx,  ny,  nz) glNormal3dDEBUG( nx,  ny,  nz, __FILE__, __LINE__ )
#define glNormal3dv( v) glNormal3dvDEBUG( v, __FILE__, __LINE__ )
#define glNormal3f( nx,  ny,  nz) glNormal3fDEBUG( nx,  ny,  nz, __FILE__, __LINE__ )
#define glNormal3fv( v) glNormal3fvDEBUG( v, __FILE__, __LINE__ )
#define glNormal3i( nx,  ny,  nz) glNormal3iDEBUG( nx,  ny,  nz, __FILE__, __LINE__ )
#define glNormal3iv( v) glNormal3ivDEBUG( v, __FILE__, __LINE__ )
#define glNormal3s( nx,  ny,  nz) glNormal3sDEBUG( nx,  ny,  nz, __FILE__, __LINE__ )
#define glNormal3sv( v) glNormal3svDEBUG( v, __FILE__, __LINE__ )
#define glNormalPointer( type,  stride,  pointer) glNormalPointerDEBUG( type,  stride,  pointer, __FILE__, __LINE__ )
#define glOrtho( left,  right,  bottom,  top,  zNear,  zFar) glOrthoDEBUG( left,  right,  bottom,  top,  zNear,  zFar, __FILE__, __LINE__ )
#define glPassThrough( token) glPassThroughDEBUG( token, __FILE__, __LINE__ )
#define glPixelMapfv( map,  mapsize,  values) glPixelMapfvDEBUG( map,  mapsize,  values, __FILE__, __LINE__ )
#define glPixelMapuiv( map,  mapsize,  values) glPixelMapuivDEBUG( map,  mapsize,  values, __FILE__, __LINE__ )
#define glPixelMapusv( map,  mapsize,  values) glPixelMapusvDEBUG( map,  mapsize,  values, __FILE__, __LINE__ )
#define glPixelStoref( pname,  param) glPixelStorefDEBUG( pname,  param, __FILE__, __LINE__ )
#define glPixelStorei( pname,  param) glPixelStoreiDEBUG( pname,  param, __FILE__, __LINE__ )
#define glPixelTransferf( pname,  param) glPixelTransferfDEBUG( pname,  param, __FILE__, __LINE__ )
#define glPixelTransferi( pname,  param) glPixelTransferiDEBUG( pname,  param, __FILE__, __LINE__ )
#define glPixelZoom( xfactor,  yfactor) glPixelZoomDEBUG( xfactor,  yfactor, __FILE__, __LINE__ )
#define glPointSize( size) glPointSizeDEBUG( size, __FILE__, __LINE__ )
#define glPolygonMode( face,  mode) glPolygonModeDEBUG( face,  mode, __FILE__, __LINE__ )
#define glPolygonOffset( factor,  units) glPolygonOffsetDEBUG( factor,  units, __FILE__, __LINE__ )
#define glPolygonStipple( mask) glPolygonStippleDEBUG( mask, __FILE__, __LINE__ )
#define glPopAttrib() glPopAttribDEBUG( __FILE__, __LINE__ )
#define glPopClientAttrib() glPopClientAttribDEBUG( __FILE__, __LINE__ )
#define glPopMatrix() glPopMatrixDEBUG( __FILE__, __LINE__ )
#define glPopName() glPopNameDEBUG( __FILE__, __LINE__ )
#define glPrioritizeTextures( n,  textures,  priorities) glPrioritizeTexturesDEBUG( n,  textures,  priorities, __FILE__, __LINE__ )
#define glPushAttrib( mask) glPushAttribDEBUG( mask, __FILE__, __LINE__ )
#define glPushClientAttrib( mask) glPushClientAttribDEBUG( mask, __FILE__, __LINE__ )
#define glPushMatrix() glPushMatrixDEBUG( __FILE__, __LINE__ )
#define glPushName( name) glPushNameDEBUG( name, __FILE__, __LINE__ )
#define glRasterPos2d( x,  y) glRasterPos2dDEBUG( x,  y, __FILE__, __LINE__ )
#define glRasterPos2dv( v) glRasterPos2dvDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos2f( x,  y) glRasterPos2fDEBUG( x,  y, __FILE__, __LINE__ )
#define glRasterPos2fv( v) glRasterPos2fvDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos2i( x,  y) glRasterPos2iDEBUG( x,  y, __FILE__, __LINE__ )
#define glRasterPos2iv( v) glRasterPos2ivDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos2s( x,  y) glRasterPos2sDEBUG( x,  y, __FILE__, __LINE__ )
#define glRasterPos2sv( v) glRasterPos2svDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos3d( x,  y,  z) glRasterPos3dDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glRasterPos3dv( v) glRasterPos3dvDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos3f( x,  y,  z) glRasterPos3fDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glRasterPos3fv( v) glRasterPos3fvDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos3i( x,  y,  z) glRasterPos3iDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glRasterPos3iv( v) glRasterPos3ivDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos3s( x,  y,  z) glRasterPos3sDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glRasterPos3sv( v) glRasterPos3svDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos4d( x,  y,  z,  w) glRasterPos4dDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glRasterPos4dv( v) glRasterPos4dvDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos4f( x,  y,  z,  w) glRasterPos4fDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glRasterPos4fv( v) glRasterPos4fvDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos4i( x,  y,  z,  w) glRasterPos4iDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glRasterPos4iv( v) glRasterPos4ivDEBUG( v, __FILE__, __LINE__ )
#define glRasterPos4s( x,  y,  z,  w) glRasterPos4sDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glRasterPos4sv( v) glRasterPos4svDEBUG( v, __FILE__, __LINE__ )
#define glReadBuffer( mode) glReadBufferDEBUG( mode, __FILE__, __LINE__ )
#define glReadPixels( x,  y,  width,  height,  format,  type,  pixels) glReadPixelsDEBUG( x,  y,  width,  height,  format,  type,  pixels, __FILE__, __LINE__ )
#define glRectd( x1,  y1,  x2,  y2) glRectdDEBUG( x1,  y1,  x2,  y2, __FILE__, __LINE__ )
#define glRectdv( v1,  v2) glRectdvDEBUG( v1,  v2, __FILE__, __LINE__ )
#define glRectf( x1,  y1,  x2,  y2) glRectfDEBUG( x1,  y1,  x2,  y2, __FILE__, __LINE__ )
#define glRectfv( v1,  v2) glRectfvDEBUG( v1,  v2, __FILE__, __LINE__ )
#define glRecti( x1,  y1,  x2,  y2) glRectiDEBUG( x1,  y1,  x2,  y2, __FILE__, __LINE__ )
#define glRectiv( v1,  v2) glRectivDEBUG( v1,  v2, __FILE__, __LINE__ )
#define glRects( x1,  y1,  x2,  y2) glRectsDEBUG( x1,  y1,  x2,  y2, __FILE__, __LINE__ )
#define glRectsv( v1,  v2) glRectsvDEBUG( v1,  v2, __FILE__, __LINE__ )
#define glRenderMode( mode) glRenderModeDEBUG( mode, __FILE__, __LINE__ )
#define glRotated( angle,  x,  y,  z) glRotatedDEBUG( angle,  x,  y,  z, __FILE__, __LINE__ )
#define glRotatef( angle,  x,  y,  z) glRotatefDEBUG( angle,  x,  y,  z, __FILE__, __LINE__ )
#define glScaled( x,  y,  z) glScaledDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glScalef( x,  y,  z) glScalefDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glScissor( x,  y,  width,  height) glScissorDEBUG( x,  y,  width,  height, __FILE__, __LINE__ )
#define glSelectBuffer( size,  buffer) glSelectBufferDEBUG( size,  buffer, __FILE__, __LINE__ )
#define glShadeModel( mode) glShadeModelDEBUG( mode, __FILE__, __LINE__ )
#define glStencilFunc( func,  ref,  mask) glStencilFuncDEBUG( func,  ref,  mask, __FILE__, __LINE__ )
#define glStencilMask( mask) glStencilMaskDEBUG( mask) glStencilMaskDEBUG( mask) glStencilMaskDEBUG( mask, __FILE__, __LINE__ )
#define glStencilOp( fail,  zfail,  zpass) glStencilOpDEBUG( fail,  zfail,  zpass, __FILE__, __LINE__ )
#define glTexCoord1d( s) glTexCoord1dDEBUG( s) glTexCoord1dDEBUG( s) glTexCoord1dDEBUG( s, __FILE__, __LINE__ )
#define glTexCoord1dv( v) glTexCoord1dvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord1f( s) glTexCoord1fDEBUG( s, __FILE__, __LINE__ )
#define glTexCoord1fv( v) glTexCoord1fvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord1i( s) glTexCoord1iDEBUG( s, __FILE__, __LINE__ )
#define glTexCoord1iv( v) glTexCoord1ivDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord1s( s) glTexCoord1sDEBUG( s, __FILE__, __LINE__ )
#define glTexCoord1sv( v) glTexCoord1svDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord2d( s,  t) glTexCoord2dDEBUG( s,  t, __FILE__, __LINE__ )
#define glTexCoord2dv( v) glTexCoord2dvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord2f( s,  t) glTexCoord2fDEBUG( s,  t, __FILE__, __LINE__ )
#define glTexCoord2fv( v) glTexCoord2fvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord2i( s,  t) glTexCoord2iDEBUG( s,  t, __FILE__, __LINE__ )
#define glTexCoord2iv( v) glTexCoord2ivDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord2s( s,  t) glTexCoord2sDEBUG( s,  t, __FILE__, __LINE__ )
#define glTexCoord2sv( v) glTexCoord2svDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord3d( s,  t,  r) glTexCoord3dDEBUG( s,  t,  r, __FILE__, __LINE__ )
#define glTexCoord3dv( v) glTexCoord3dvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord3f( s,  t,  r) glTexCoord3fDEBUG( s,  t,  r, __FILE__, __LINE__ )
#define glTexCoord3fv( v) glTexCoord3fvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord3i( s,  t,  r) glTexCoord3iDEBUG( s,  t,  r, __FILE__, __LINE__ )
#define glTexCoord3iv( v) glTexCoord3ivDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord3s( s,  t,  r) glTexCoord3sDEBUG( s,  t,  r, __FILE__, __LINE__ )
#define glTexCoord3sv( v) glTexCoord3svDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord4d( s,  t,  r,  q) glTexCoord4dDEBUG( s,  t,  r,  q, __FILE__, __LINE__ )
#define glTexCoord4dv( v) glTexCoord4dvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord4f( s,  t,  r,  q) glTexCoord4fDEBUG( s,  t,  r,  q, __FILE__, __LINE__ )
#define glTexCoord4fv( v) glTexCoord4fvDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord4i( s,  t,  r,  q) glTexCoord4iDEBUG( s,  t,  r,  q, __FILE__, __LINE__ )
#define glTexCoord4iv( v) glTexCoord4ivDEBUG( v) glTexCoord4ivDEBUG( v) glTexCoord4ivDEBUG( v, __FILE__, __LINE__ )
#define glTexCoord4s( s,  t,  r,  q) glTexCoord4sDEBUG( s,  t,  r,  q, __FILE__, __LINE__ )
#define glTexCoord4sv( v) glTexCoord4svDEBUG( v, __FILE__, __LINE__ )
#define glTexCoordPointer( size,  type,  stride,  pointer) glTexCoordPointerDEBUG( size,  type,  stride,  pointer, __FILE__, __LINE__ )
#define glTexEnvf( target,  pname,  param) glTexEnvfDEBUG( target,  pname,  param, __FILE__, __LINE__ )
#define glTexEnvfv( target,  pname,  params) glTexEnvfvDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glTexEnvi( target,  pname,  param) glTexEnviDEBUG( target,  pname,  param, __FILE__, __LINE__ )
#define glTexEnviv( target,  pname,  params) glTexEnvivDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glTexGend( coord,  pname,  param) glTexGendDEBUG( coord,  pname,  param, __FILE__, __LINE__ )
#define glTexGendv( coord,  pname,  params) glTexGendvDEBUG( coord,  pname,  params, __FILE__, __LINE__ )
#define glTexGenf( coord,  pname,  param) glTexGenfDEBUG( coord,  pname,  param, __FILE__, __LINE__ )
#define glTexGenfv( coord,  pname,  params) glTexGenfvDEBUG( coord,  pname,  params, __FILE__, __LINE__ )
#define glTexGeni( coord,  pname,  param) glTexGeniDEBUG( coord,  pname,  param, __FILE__, __LINE__ )
#define glTexGeniv( coord,  pname,  params) glTexGenivDEBUG( coord,  pname,  params, __FILE__, __LINE__ )
#define glTexImage1D( target,  level,  internalformat,  width,  border,  format,  type,  pixels) glTexImage1DDEBUG( target,  level,  internalformat,  width,  border,  format,  type,  pixels, __FILE__, __LINE__ )
#define glTexImage2D( target,  level,  internalformat,  width,  height,  border,  format,  type,  pixels) glTexImage2DDEBUG( target,  level,  internalformat,  width,  height,  border,  format,  type,  pixels, __FILE__, __LINE__ )
#define glTexParameterf( target,  pname,  param) glTexParameterfDEBUG( target,  pname,  param, __FILE__, __LINE__ )
#define glTexParameterfv( target,  pname,  params) glTexParameterfvDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glTexParameteri( target,  pname,  param) glTexParameteriDEBUG( target,  pname,  param, __FILE__, __LINE__ )
#define glTexParameteriv( target,  pname,  params) glTexParameterivDEBUG( target,  pname,  params, __FILE__, __LINE__ )
#define glTexSubImage1D( target,  level,  xoffset,  width,  format,  type,  pixels) glTexSubImage1DDEBUG( target,  level,  xoffset,  width,  format,  type,  pixels, __FILE__, __LINE__ )
#define glTexSubImage2D( target,  level,  xoffset,  yoffset,  width,  height,  format,  type,  pixels) glTexSubImage2DDEBUG( target,  level,  xoffset,  yoffset,  width,  height,  format,  type,  pixels, __FILE__, __LINE__ )
#define glTranslated( x,  y,  z) glTranslatedDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glTranslatef( x,  y,  z) glTranslatefDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glVertex2d( x,  y) glVertex2dDEBUG( x,  y, __FILE__, __LINE__ )
#define glVertex2dv( v) glVertex2dvDEBUG( v, __FILE__, __LINE__ )
#define glVertex2f( x,  y) glVertex2fDEBUG( x,  y, __FILE__, __LINE__ )
#define glVertex2fv( v) glVertex2fvDEBUG( v, __FILE__, __LINE__ )
#define glVertex2i( x,  y) glVertex2iDEBUG( x,  y, __FILE__, __LINE__ )
#define glVertex2iv( v) glVertex2ivDEBUG( v, __FILE__, __LINE__ )
#define glVertex2s( x,  y) glVertex2sDEBUG( x,  y, __FILE__, __LINE__ )
#define glVertex2sv( v) glVertex2svDEBUG( v, __FILE__, __LINE__ )
#define glVertex3d( x,  y,  z) glVertex3dDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glVertex3dv( v) glVertex3dvDEBUG( v, __FILE__, __LINE__ )
#define glVertex3f( x,  y,  z) glVertex3fDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glVertex3fv( v) glVertex3fvDEBUG( v, __FILE__, __LINE__ )
#define glVertex3i( x,  y,  z) glVertex3iDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glVertex3iv( v) glVertex3ivDEBUG( v, __FILE__, __LINE__ )
#define glVertex3s( x,  y,  z) glVertex3sDEBUG( x,  y,  z, __FILE__, __LINE__ )
#define glVertex3sv( v) glVertex3svDEBUG( v, __FILE__, __LINE__ )
#define glVertex4d( x,  y,  z,  w) glVertex4dDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glVertex4dv( v) glVertex4dvDEBUG( v, __FILE__, __LINE__ )
#define glVertex4f( x,  y,  z,  w) glVertex4fDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glVertex4fv( v) glVertex4fvDEBUG( v, __FILE__, __LINE__ )
#define glVertex4i( x,  y,  z,  w) glVertex4iDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glVertex4iv( v) glVertex4ivDEBUG( v, __FILE__, __LINE__ )
#define glVertex4s( x,  y,  z,  w) glVertex4sDEBUG( x,  y,  z,  w, __FILE__, __LINE__ )
#define glVertex4sv( v) glVertex4svDEBUG( v, __FILE__, __LINE__ )
#define glVertexPointer( size,  type,  stride,  pointer) glVertexPointerDEBUG( size,  type,  stride,  pointer, __FILE__, __LINE__ )
#define glViewport( x,  y,  width,  height) glViewportDEBUG( x,  y,  width,  height, __FILE__, __LINE__ )
#endif / GLDRIVER_DEBUG_MODE ? /

/*--------------------------------------------------------------------------*/

/*
 *      Macros that redirect opengl 1.5 function calls to the debug versions of
 *      the functions with added trace information.
 */
#ifdef GLDRIVER_DEBUG_MODE
#define glBeginQuery( target, id ) glBeginQueryDEBUG( target, id, __FILE__, __LINE__ )
#define glBindBuffer( target, buffer ) glBindBufferDEBUG( target, buffer, __FILE__, __LINE__ )
#endif /* GLDRIVER_DEBUG_MODE ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangling
 */
#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*/

#ifdef GLDRIVER_DYNAMIC_OPENGL_LINKING

/**
 *      Function that attempts to link the given library and set up all the
 *      gl* function pointers. If successfull GLDRIVER_TRUE is returned,
 *      otherwise an error code is returned. Note that failure to load a function
 *      belonging to OpenGL version 1.2 and higher is not considdered an error.
 *      Any driver that is already loaded will be unloaded.
 */
GLDRIVER_EXPORT int
glLoadDriver( const char *library );

/**
 *      Unloads any GL library that may have been loaded with a successfull
 *      call to glLoadDriver(). This will invalidate all the gl* function
 *      pointers !!!.
 */
GLDRIVER_EXPORT void
glUnloadDriver( void );

/**
 *      Returns wheter or not a GL driver has been loaded. If not then the
 *      gl* function pointers should not be used. GLDRIVER_TRUE indicates true
 *      any other value indicates false and possibly an error that occured
 *      during the last glLoadDriver() attempt.
 */
GLDRIVER_EXPORT int
glIsDriverLoaded( void );

#endif /* GLDRIVER_DYNAMIC_OPENGL_LINKING ? */

/*-------------------------------------------------------------------------*/

/**
 *      Returns a const pointer to a null-terminated string holding the current
 *      driver library string. This function can return NULL if no driver library
 *      is loaded.
 */
GLDRIVER_EXPORT const char*
glDriverLibrary( void );

/**
 *      Returns the version of the OpenGL driver that was loaded.
 *      OpenGL functions will be loaded incrementally from version 1.1 up.
 *      If just one of the functions in a set can't be loaded the library will
 *      be considdered to be of the version belonging to the last successfull
 *      function set load. If no driver has been loaded the return value will be
 *      zero.
 */
GLDRIVER_EXPORT GLfloat
glVersion( void );

/*-------------------------------------------------------------------------*/

#ifdef GLDRIVER_DEBUG_MODE

/**
 *  prototype definition of the OpenGL error callback function.
 */
typedef void ( GLDRIVER_CALLSPEC_PREFIX *TOGLErrorCallback)( GLenum, const char*, const char*, UInt32 ) GLDRIVER_CALLSPEC_SUFFIX;

/**
 *      Sets the error callback function.
 *      Only available in debug mode !!!
 *      The callback function will be called if a call to a gl function
 *      generated a gl error. Trace information will be provided.
 *      the callbackfunc should have the following protoype for BCB or MSVC:
 *              void __stdcall myfunc( GLenum error, const char* glfunc, const char* file, UInt32 line );
 *      and the following prototype for other compilers:
 *              void myfunc( GLenum error, const char* glfunc, const char* file, UInt32 line ) __attribute__((stdcall));
 *      You can offcourse use a different function name then myfunc.
 */
GLDRIVER_EXPORT void
glErrorHandler( TOGLErrorCallback callback );

#endif /* GLDRIVER_DEBUG_MODE ? */

/*-------------------------------------------------------------------------*/

/*
 *      End the C++ name mangling protection
 */
#ifdef __cplusplus
};
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Here we undef the macros so they dont affect other headers.
 */
#undef GLDRIVER_DEBUG_EXPORT_C
#undef GLDRIVER_EXPORT_C
#undef GLDRIVER_EXPORT
#undef UInt32

/*--------------------------------------------------------------------------*/

#endif /* GLDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-09-2004 :
        - Started adding debug versions all OpenGL functions. Combined with
          new macros this allows me to see which gl function sets the error
          state plus trace to where the call originated. An optional user
          callback is provided to do some extra error handling if desired.
          Maybe throw an exception in C++, always make sure you have 0 gl errors
          in your app code or you may get unexpected results.
- 29-08-2004 :
        - Added support for OpenGL 1.5
- 28-08-2004 :
        - Finished adding support for OpenGL versions. Support is now at
          version 1.4, extension loading will be added later.
- 25-08-2004 :
        - Started adding support for loading extensions and OpenGL functionality
          newer then version 1.1 The OpenGL header include will be removed and
          this header will completely replace it. 
- 05-07-2004 :
        - Created this set of function pointers and loader so that dynamic
          linking to an OpenGL implementation becomes possible and that we can
          do so with ease. Besides the standard system GL driver Mesa3D can
          be an interesting alternative, its software renderer could be usefull
          for tracking down bugs.

-----------------------------------------------------------------------------*/
