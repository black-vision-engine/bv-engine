#pragma once

#include "CoreDEF.h"

namespace bv
{

struct SceneNodePerformance
{
    double      GPURenderDuration;
    double      CPURenderDuration;
    double      SortNodeDuration;

    UInt64      GPURenderAccum;
    double      CPURenderAccum;
    double      SortNodeAccum;


    SceneNodePerformance()
    {
        GPURenderDuration = 0;
        CPURenderDuration = 0;
        SortNodeDuration = 0;

        GPURenderAccum = 0;
        CPURenderAccum = 0;
        SortNodeAccum = 0;
    }
};

}   // bv

