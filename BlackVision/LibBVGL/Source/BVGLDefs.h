#pragma once

#include <crtdefs.h>

#ifdef BV_GL_VERSION_4_5
	#define BV_GL_VERSION_4_4
#endif

namespace bv {

#define GL_FALSE 0
#define GL_TRUE 1

#define GL_ZERO 0
#define GL_ONE 1

#define GL_VENDOR											0x1F00
#define GL_RENDERER											0x1F01
#define GL_VERSION											0x1F02
#define GL_EXTENSIONS										0x1F03

#define GL_SHADING_LANGUAGE_VERSION							0x8B8C

#define GL_MAJOR_VERSION									0x821B
#define GL_MINOR_VERSION									0x821C
#define GL_NUM_EXTENSIONS									0x821D

#define GL_MAX_VERTEX_ATTRIBS								0x8869
#define GL_MAX_TEXTURE_UNITS								0x84E2
#define GL_MAX_TEXTURE_SIZE									0x0D33
#define GL_MAX_3D_TEXTURE_SIZE								0x8073
#define GL_MAX_DRAW_BUFFERS									0x8824
#define GL_MAX_VARYING_FLOATS								0x8B4B
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS					0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS					0x8B4A
#define GL_MAX_VARYING_FLOATS								0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS					0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS					0x8B4D
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS					0x8C29
#define GL_MAX_TEXTURE_IMAGE_UNITS							0x8872

#define GL_STENCIL_TEST										0x0B90
#define GL_CURRENT_PROGRAM                                  0x8B8D

#define GL_NO_ERROR 0
#define GL_INVALID_ENUM										0x0500
#define GL_INVALID_VALUE									0x0501
#define GL_INVALID_OPERATION								0x0502
#define GL_INVALID_FRAMEBUFFER_OPERATION					0x0506
#define GL_OUT_OF_MEMORY									0x0505

#define GL_BYTE							0x1400
#define GL_UNSIGNED_BYTE				0x1401
#define GL_SHORT						0x1402
#define GL_UNSIGNED_SHORT				0x1403
#define GL_INT							0x1404
#define GL_UNSIGNED_INT					0x1405
#define GL_FLOAT						0x1406
#define GL_UNSIGNED_BYTE_3_3_2			0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4		0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1		0x8034
#define GL_UNSIGNED_INT_8_8_8_8			0x8035
#define GL_UNSIGNED_INT_10_10_10_2		0x8036
#define GL_UNSIGNED_BYTE_2_3_3_REV		0x8362
#define GL_UNSIGNED_SHORT_5_6_5			0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV		0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV	0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV	0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV		0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV	0x8368

// internal formats
#define GL_RED_SNORM					0x8F90
#define GL_RG_SNORM						0x8F91
#define GL_RGB_SNORM					0x8F92
#define GL_RGBA_SNORM					0x8F93
#define GL_R8_SNORM						0x8F94
#define GL_RG8_SNORM					0x8F95
#define GL_RGB8_SNORM					0x8F96
#define GL_RGBA8_SNORM					0x8F97
#define GL_R16_SNORM					0x8F98
#define GL_RG16_SNORM					0x8F99
#define GL_RGB16_SNORM					0x8F9A
#define GL_RGBA16_SNORM					0x8F9B
#define GL_COMPRESSED_RED				0x8225
#define GL_COMPRESSED_RG				0x8226
#define GL_RG							0x8227
#define GL_RG_INTEGER					0x8228
#define GL_R8							0x8229
#define GL_R16							0x822A
#define GL_RG8							0x822B
#define GL_RG16							0x822C
#define GL_R16F							0x822D
#define GL_R32F							0x822E
#define GL_RG16F						0x822F
#define GL_RG32F						0x8230
#define GL_R8I							0x8231
#define GL_R8UI							0x8232
#define GL_R16I							0x8233
#define GL_R16UI						0x8234
#define GL_R32I							0x8235
#define GL_R32UI						0x8236
#define GL_RG8I							0x8237
#define GL_RG8UI						0x8238
#define GL_RG16I						0x8239
#define GL_RG16UI						0x823A
#define GL_RG32I						0x823B
#define GL_RG32UI						0x823C
#define GL_RGBA32UI						0x8D70
#define GL_RGB32UI						0x8D71
#define GL_RGBA16UI						0x8D76
#define GL_RGB16UI						0x8D77
#define GL_RGBA8UI						0x8D7C
#define GL_RGB8UI						0x8D7D
#define GL_RGBA32I						0x8D82
#define GL_RGB32I						0x8D83
#define GL_RGBA16I						0x8D88
#define GL_RGB16I						0x8D89
#define GL_RGBA8I						0x8D8E
#define GL_RGB8I						0x8D8F
#define GL_RGB4							0x804F
#define GL_RGB5							0x8050
#define GL_RGB8							0x8051
#define GL_RGB10						0x8052
#define GL_RGB12						0x8053
#define GL_RGB16						0x8054
#define GL_RGBA2						0x8055
#define GL_RGBA4						0x8056
#define GL_RGB5_A1						0x8057
#define GL_RGBA8						0x8058
#define GL_RGB10_A2						0x8059
#define GL_RGBA12						0x805A
#define GL_RGBA16						0x805B
#define GL_R11F_G11F_B10F				0x8C3A
#define GL_RGB9_E5						0x8C3D

#define GL_COMPRESSED_RED				0x8225
#define GL_COMPRESSED_RG				0x8226
#define GL_COMPRESSED_RED_RGTC1			0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1	0x8DBC
#define GL_COMPRESSED_RG_RGTC2			0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2	0x8DBE
#define GL_COMPRESSED_RGB				0x84ED
#define GL_COMPRESSED_RGBA				0x84EE
#define GL_COMPRESSED_SRGB				0x8C48
#define GL_COMPRESSED_SRGB_ALPHA		0x8C49
#define GL_COMPRESSED_RGBA_BPTC_UNORM			0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM		0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT		0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	0x8E8F


#define GL_VERTEX_SHADER						0x8B31
#define GL_FRAGMENT_SHADER						0x8B30
#define GL_GEOMETRY_SHADER						0x8DD9
#define GL_TESS_EVALUATION_SHADER				0x8E87
#define GL_TESS_CONTROL_SHADER					0x8E88

#define GL_COMPILE_STATUS						0x8B81
#define GL_INFO_LOG_LENGTH						0x8B84
#define GL_LINK_STATUS							0x8B82
#define GL_ACTIVE_UNIFORMS						0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH			0x8B87
#define GL_ACTIVE_ATTRIBUTES					0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH			0x8B8A
#define GL_VALIDATE_STATUS						0x8B83
#define GL_READ_ONLY							0x88B8
#define GL_WRITE_ONLY							0x88B9

// Texture binding targets
#define GL_TEXTURE_1D							0x0DE0
#define GL_TEXTURE_2D							0x0DE1
#define GL_TEXTURE_3D							0x806F
#define GL_TEXTURE_1D_ARRAY						0x8C18
#define GL_TEXTURE_2D_ARRAY						0x8C1A
#define GL_TEXTURE_CUBE_MAP						0x8513
#define GL_TEXTURE_CUBE_MAP_ARRAY				0x9009
#define GL_TEXTURE_2D_MULTISAMPLE				0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY			0x9102
#define GL_TEXTURE_BUFFER						0x8C2A

#define GL_TEXTURE_CUBE_MAP_POSITIVE_X			0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X			0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y			0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y			0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z			0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z			0x851A

#define GL_TEXTURE_BINDING_1D					0x8068
#define GL_TEXTURE_BINDING_2D					0x8069
#define GL_TEXTURE_BINDING_3D					0x806A
#define GL_TEXTURE_BINDING_1D_ARRAY				0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY				0x8C1D
#define GL_TEXTURE_BINDING_CUBE_MAP				0x8514
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY		0x900A

#define GL_SAMPLER_BINDING						0x8919

#define	GL_TEXTURE_CUBE_MAP_SEAMLESS			0x884F

#define GL_TEXTURE_BORDER_COLOR					0x1004
#define GL_READ_WRITE					0x88BA
#define GL_STREAM_DRAW					0x88E0
#define GL_STREAM_READ					0x88E1
#define GL_STREAM_COPY					0x88E2
#define GL_DYNAMIC_DRAW					0x88E8
#define GL_DYNAMIC_READ					0x88E9
#define GL_DYNAMIC_COPY					0x88EA
#define GL_STATIC_DRAW					0x88E4
#define GL_STATIC_READ					0x88E5
#define GL_STATIC_COPY					0x88E6

// Buffer binding target
#define GL_ARRAY_BUFFER					0x8892
#define GL_ELEMENT_ARRAY_BUFFER			0x8893
#define GL_DRAW_INDIRECT_BUFFER			0x8F3F
#define GL_UNIFORM_BUFFER				0x8A11
#define GL_TRANSFORM_FEEDBACK_BUFFER	0x8C8E
#define GL_PIXEL_PACK_BUFFER			0x88EB
#define GL_PIXEL_UNPACK_BUFFER			0x88EC
#define GL_SHADER_STORAGE_BUFFER		0x90D2
#define GL_TEXTURE_BUFFER				0x8C2A
#define GL_COPY_READ_BUFFER				0x8F36
#define GL_COPY_WRITE_BUFFER			0x8F37
#define GL_ATOMIC_COUNTER_BUFFER		0x92C0
//	// todo
//#define GL_DISPATCH_INDIRECT_BUFFER
//#define GL_QUERY_BUFFER
//#define GL_SHADER_STORAGE_BUFFER

// Topology
#define GL_TRIANGLES					0x0004
#define GL_TRIANGLE_STRIP				0x0005
#define GL_POINTS						0x0000
#define GL_LINES						0x0001
#define GL_LINE_STRIP					0x0003
#define GL_LINE_LOOP					0x0002
#define GL_LINE_STRIP_ADJACENCY			0x000B
#define GL_LINES_ADJACENCY				0x000A
#define GL_TRIANGLES_ADJACENCY			0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY		0x000D
#define GL_PATCHES						0x000E
#define GL_TRIANGLE_FAN					0x0006

// MipMap filtering
#define GL_NEAREST						0x2600
#define GL_LINEAR						0x2601
#define GL_NEAREST_MIPMAP_NEAREST		0x2700
#define GL_LINEAR_MIPMAP_NEAREST		0x2701
#define GL_NEAREST_MIPMAP_LINEAR		0x2702
#define GL_LINEAR_MIPMAP_LINEAR			0x2703
// Filtering type
#define GL_TEXTURE_MAG_FILTER			0x2800
#define GL_TEXTURE_MIN_FILTER			0x2801
// Wraping mode
#define GL_TEXTURE_WRAP_S				0x2802
#define GL_TEXTURE_WRAP_T				0x2803
#define GL_TEXTURE_WRAP_R				0x8072

#define GL_CLAMP						0x2900
#define GL_REPEAT						0x2901
#define GL_MIRRORED_REPEAT				0x8370
#define GL_CLAMP_TO_BORDER				0x812D
#define GL_CLAMP_TO_EDGE				0x812F
// // todo
//#define GL_MIRROR_CLAMP_TO_EDGE

#define GL_TEXTURE_BASE_LEVEL			0x813C
#define GL_TEXTURE_MAX_LEVEL			0x813D

#define GL_TEXTURE0						0x84C0


#define GL_FRONT_LEFT					0x0400
#define GL_FRONT_RIGHT					0x0401
#define GL_BACK_LEFT					0x0402
#define GL_BACK_RIGHT					0x0403
#define GL_COLOR_ATTACHMENT0			0x8CE0
#define GL_COLOR_ATTACHMENT1			0x8CE1
#define GL_COLOR_ATTACHMENT2			0x8CE2
#define GL_COLOR_ATTACHMENT3			0x8CE3
#define GL_COLOR_ATTACHMENT4			0x8CE4
#define GL_COLOR_ATTACHMENT5			0x8CE5
#define GL_COLOR_ATTACHMENT6			0x8CE6
#define GL_COLOR_ATTACHMENT7			0x8CE7
#define GL_COLOR_ATTACHMENT8			0x8CE8
#define GL_COLOR_ATTACHMENT9			0x8CE9
#define GL_COLOR_ATTACHMENT10			0x8CEA
#define GL_COLOR_ATTACHMENT11			0x8CEB
#define GL_COLOR_ATTACHMENT12			0x8CEC
#define GL_COLOR_ATTACHMENT13			0x8CED
#define GL_COLOR_ATTACHMENT14			0x8CEE
#define GL_COLOR_ATTACHMENT15			0x8CEF
#define GL_DEPTH_ATTACHMENT				0x8D00
#define GL_STENCIL_ATTACHMENT			0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT		0x821A


#define GL_DEPTH_COMPONENT				0x1902

#define GL_BLEND						0x0BE2

#define GL_SRC_COLOR					0x0300
#define GL_ONE_MINUS_SRC_COLOR			0x0301
#define GL_SRC_ALPHA					0x0302
#define GL_ONE_MINUS_SRC_ALPHA			0x0303
#define GL_DST_ALPHA					0x0304
#define GL_ONE_MINUS_DST_ALPHA			0x0305
#define GL_DST_COLOR					0x0306
#define GL_ONE_MINUS_DST_COLOR			0x0307
#define GL_SRC_ALPHA_SATURATE			0x0308
#define GL_CONSTANT_COLOR				0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR		0x8002
#define GL_CONSTANT_ALPHA				0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA		0x8004

#define GL_NEVER						0x0200
#define GL_LESS							0x0201
#define GL_EQUAL						0x0202
#define GL_LEQUAL						0x0203
#define GL_GREATER						0x0204
#define GL_NOTEQUAL						0x0205
#define GL_GEQUAL						0x0206
#define GL_ALWAYS						0x0207

//pablito
#define GL_RGB10 0x8052
#define GL_POINT						0x1B00
#define GL_LINE							0x1B01
#define GL_FILL							0x1B02

#define GL_RGB16 0x8054
#define GL_RGBA16 0x805B
//#define GL_ALPHA16
#define GL_BGR							0x80E0
#define GL_BGRA							0x80E1
#define GL_RGBA							0x1908
#define GL_ALPHA						0x1906
#define GL_LUMINANCE					0x1909
#define GL_R32F							0x822E
#define GL_RGBA32F						0x8814
#define GL_RGB32F						0x8815

#define GL_ALPHA8						0x803C
#define GL_LUMINANCE8					0x8040

#define GL_READ_FRAMEBUFFER				0x8CA8
#define GL_DRAW_FRAMEBUFFER				0x8CA9
#define GL_FRAMEBUFFER					0x8D40
#define GL_RENDERBUFFER					0x8D41

#define GL_FRAMEBUFFER_COMPLETE			0x8CD5

#define GL_TRANSFORM_FEEDBACK			0x8E22

#define GL_DEPTH_RANGE					0x0B70
#define GL_DEPTH_TEST					0x0B71

#define GL_POLYGON_OFFSET_POINT			0x2A01
#define GL_POLYGON_OFFSET_LINE			0x2A02

#define GL_POLYGON_OFFSET_FILL			0x8037
#define GL_POLYGON_OFFSET_FACTOR		0x8038

#define GL_FRONT						0x0404
#define GL_BACK							0x0405
#define GL_FRONT_AND_BACK				0x0408
#define GL_CW							0x0900
#define GL_CCW							0x0901
#define GL_CULL_FACE					0x0B44
#define GL_CULL_FACE_MODE				0x0B45

#define GL_BOOL                         0x8B56
#define GL_FLOAT_VEC2                   0x8B50
#define GL_FLOAT_VEC3                   0x8B51
#define GL_FLOAT_VEC4                   0x8B52
#define GL_FLOAT_MAT2                   0x8B5A
#define GL_FLOAT_MAT3                   0x8B5B
#define GL_FLOAT_MAT4                   0x8B5C

#define GL_UNIFORM_TYPE                         0x8A37
#define GL_UNIFORM_SIZE                         0x8A38
#define GL_UNIFORM_OFFSET                       0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE                 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE                0x8A3D
#define GL_UNIFORM_BLOCK_DATA_SIZE              0x8A40
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS        0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43

#define GL_INVALID_INDEX                0xFFFFFFFF

//pablito
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_RENDERBUFFER_EXT 0x8D41
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_VIEWPORT						0x0BA2
#define GL_COLOR_BUFFER_BIT				0x00004000
#define GL_DEPTH_BUFFER_BIT				0x00000100


// Query
#define GL_TIMESTAMP                    0x8E28
#define GL_QUERY_RESULT                 0x8866


// Buffer access
#define GL_MAP_INVALIDATE_BUFFER_BIT    0x0008
#define GL_MAP_INVALIDATE_RANGE_BIT     0x0004
#define GL_MAP_WRITE_BIT                0x0002



typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned long long int GLuint64;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef char GLchar;

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
// FIXME: remove later

} //bv
