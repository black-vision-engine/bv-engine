#pragma once

#include "System/BasicTypes.h" //FIXME: store in PCH


namespace bv { namespace model {

enum class TimelineOutBehavior : int
{
    TOB_CLAMP = 0,
    TOB_REPEAT,
    TOB_PINGPONG,
    TOB_CONST_VAL,

    TOB_TOTAl
};

struct TimelineSettings
{
    TimelineOutBehavior     m_preBehavior;
    TimelineOutBehavior     m_postBehavior;

    TimeType                m_constValPre;
    TimeType                m_constValPost;

};

    //FIXME: timeline state
    //FIXME: timeline logic (start, stop, pause, rev and so on)

} //model
} //bv
