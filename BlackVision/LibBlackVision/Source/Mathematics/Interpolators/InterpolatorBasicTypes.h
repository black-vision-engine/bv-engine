#pragma once

namespace bv
{

enum class WrapMethod : int
{
    clamp = 0,
    repeat = 1,
    pingPong = 2,
};

enum CurveType 
{ 
    CT_POINT = 0, 
    CT_LINEAR, 
    CT_BEZIER,
    CT_COSINE_LIKE,

    CT_CUBIC_IN,
    CT_CUBIC_OUT,
    CT_ELASTIC_IN,
    CT_ELASTIC_OUT,

    //CT_CUBIC_IN_BOUNCE,
    //CT_CUBIC_OUT_BOUNCE,
    CT_ELASTIC_IN_BOUNCE,
    CT_ELASTIC_OUT_BOUNCE,

    CT_QUARTIC_INOUT,
    CT_CUBIC_INTOUT,

    CT_TOTAL
};

enum class EvaluatorType : int
{ 
    ET_CONSTANT, 
    ET_LINEAR, 
    ET_BEZIER,
    ET_POLYNOMIAL
};


}
