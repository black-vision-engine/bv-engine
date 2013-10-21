#pragma once

namespace bv {

// ************************* STATE ALPHA *************************
enum class AlphaSrcBlendMode : int
{
    ASBM_ZERO = 0,
    ASBM_ONE,
    ASBM_DST_COLOR,
    ASBM_ONE_MINUS_DST_COLOR,
    ASBM_SRC_ALPHA,
    ASBM_ONE_MINUS_SRC_ALPHA,
    ASBM_DST_ALPHA,
    ASBM_ONE_MINUS_DST_ALPHA,
    ASBM_SRC_ALPHA_SATURATE,
    ASBM_CONSTANT_COLOR,
    ASBM_ONE_MINUS_CONSTANT_COLOR,
    ASBM_CONSTANT_ALPHA,
    ASBM_ONE_MINUS_CONSTANT_ALPHA,

    ASBM_TOTAL
};

enum class AlphaDstBlendMode : int
{
    ADBM_ZERO = 0,
    ADBM_ONE,
    ADBM_SRC_COLOR,
    ADBM_ONE_MINUS_SRC_COLOR,
    ADBM_SRC_ALPHA,
    ADBM_ONE_MINUS_SRC_ALPHA,
    ADBM_DST_ALPHA,
    ADBM_ONE_MINUS_DST_ALPHA,
    ADBM_CONSTANT_COLOR,
    ADBM_ONE_MINUS_CONSTANT_COLOR,
    ADBM_CONSTANT_ALPHA,
    ADBM_ONE_MINUS_CONSTANT_ALPHA,

    ADBM_TOTAL
};

enum AlphaCompareMode : int
{
    ACM_NEVER = 0,
    ACM_LESS,
    ACM_EQUAL,
    ACM_LEQUAL,
    ACM_GREATER,
    ACM_NOTEQUAL,
    ACM_GEQUAL,
    ACM_ALWAYS,

    ACM_TOTAL
};

// ************************* STATE DEPTH *************************
enum class DepthCompareMode : int
{
    DCM_NEVER = 0,
    DCM_LESS,
    DCM_EQUAL,
    DCM_LEQUAL,
    DCM_GREATER,
    DCM_NOTEQUAL,
    DCM_GEQUAL,
    DCM_ALWAYS,

    DCM_TOTAL
};

// ************************* STATE STENCIL *************************
enum StencilCompareMode : int
{
    SCM_NEVER = 0,
    SCM_LESS,
    SCM_EQUAL,
    SCM_LEQUAL,
    SCM_GREATER,
    SCM_NOTEQUAL,
    SCM_GEQUAL,
    SCM_ALWAYS,

    SCM_TOTAL
};

enum class StencilOperationType : int
{
    SOT_KEEP = 0,
    SOT_ZERO,
    SOT_REPLACE,
    SOT_INCREMENT,
    SOT_DECREMENT,
    SOT_INVERT,

    SOT_TOTAL
};

// ************************* STATE FILL *************************
enum FillStateMode : int
{
    FSM_POINTS = 0,
    FSM_LINES,
    FSM_POLYGONS,

    FSM_TOTAL
};

}
