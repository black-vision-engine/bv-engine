#pragma once

#include <crtdefs.h>


namespace bv {

#define AL_INVALID                                          -1

#define AL_NONE                                             0

#define AL_FALSE                                            0
#define AL_TRUE                                             1

#define AL_NO_ERROR                                         AL_FALSE
#define AL_INVALID_NAME										0xA001
#define AL_INVALID_ENUM										0xA002
#define AL_INVALID_VALUE									0xA003
#define AL_INVALID_OPERATION								0xA004
#define AL_OUT_OF_MEMORY									0xA005
        
#define AL_GAIN                                             0x100A
#define AL_MIN_GAIN                                         0x100D
#define AL_MAX_GAIN                                         0x100E
#define AL_ORIENTATION                                      0x100F
#define AL_PITCH                                            0x1003
#define AL_POSITION                                         0x1004
#define AL_DIRECTION                                        0x1005
#define AL_VELOCITY                                         0x1006 
#define AL_LOOPING                                          0x1007
#define AL_BUFFER                                           0x1009

#define AL_SOURCE_STATE                                     0x1010
#define AL_INITIAL                                          0x1011
#define AL_PLAYING                                          0x1012
#define AL_PAUSED                                           0x1013
#define AL_STOPPED                                          0x1014

#define AL_BUFFERS_QUEUED                                   0x1015
#define AL_BUFFERS_PROCESSED                                0x1016

#define AL_SOURCE_TYPE                                      0x1027
#define AL_STATIC                                           0x1028
#define AL_STREAMING                                        0x1029
#define AL_UNDETERMINED                                     0x1030

#define AL_FORMAT_MONO8                                     0x1100
#define AL_FORMAT_MONO16                                    0x1101
#define AL_FORMAT_STEREO8                                   0x1102
#define AL_FORMAT_STEREO16                                  0x1103

#define AL_FREQUENCY                                        0x2001
#define AL_BITS                                             0x2002
#define AL_CHANNELS                                         0x2003
#define AL_SIZE                                             0x2004

typedef int             ALenum;
typedef unsigned int    ALuint;
typedef int             ALint;
typedef int             ALsizei;
typedef char            ALboolean;
typedef char            ALbyte;
typedef short           ALshort;
typedef unsigned char   ALubyte;
typedef unsigned short  ALushort;
typedef float           ALfloat;
typedef double          ALdouble;
typedef char            ALchar;
typedef void            ALvoid;

} //bv
