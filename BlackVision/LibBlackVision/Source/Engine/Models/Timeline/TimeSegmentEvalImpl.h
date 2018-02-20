#pragma once

#include <vector>
#include <functional>

#include "CoreDEF.h"
#include "Mathematics/Core/mathfuncs.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class TimeSegmentEvalImpl
{
private:

    //@see: http://en.cppreference.com/w/cpp/utility/functional/function
    typedef std::function<TimeType(TimeType)> WrapEvaluator;
    typedef std::vector< std::function<TimeType(TimeType)> > WrapEvaluators;

private:

    bool                    m_started;
    bool                    m_paused;

    bool                    m_startTriggered;

    TimeType                m_globalTime;

    TimeType                m_duration;
    TimeType                m_pauseDuration;
    TimeType                m_startTime;
    TimeType                m_backwardStartTime;

    TimelinePlayDirection   m_playDirection;

    TimelineWrapMethod      m_wrapPreBehavior;
    TimelineWrapMethod      m_wrapPostBehavior;

    //FIXME: may be static
    WrapEvaluators          m_wrapEvaluatorsPre;
    WrapEvaluators          m_wrapEvaluatorsPost;

    WrapEvaluator           m_wrapEvaluatorPre;
    WrapEvaluator           m_wrapEvaluatorPost;

public:

                TimeSegmentEvalImpl ( TimeType duration, TimelinePlayDirection direction = TimelinePlayDirection::TPD_FORWAD, TimelineWrapMethod preMethod = TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod postMethod = TimelineWrapMethod::TWM_CLAMP );
                ~TimeSegmentEvalImpl();
             
    void        Start               ();
    void        Stop                ();
    void        Reverse             ();
    void        Reset               ( TimelinePlayDirection direction = TimelinePlayDirection::TPD_FORWAD );

    bool        IsActive            () const;
    bool        IsPaused            () const;

    void        UpdateGlobalTime    ( TimeType t );
    TimeType    GetLocalTime        () const;
    TimeType    GlobalToLocal       ( TimeType t ) const;

    void        SetWrapBehaviorPre  ( TimelineWrapMethod method );
    void        SetWrapBehaviorPost ( TimelineWrapMethod method );
    void        SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

    TimelineWrapMethod      GetWrapPre          () const;
    TimelineWrapMethod      GetWrapPost         () const;

    void	                SetDuration         ( TimeType duration );
    TimeType                GetDuration         () const;

    TimelinePlayDirection   GetDirection        () const;

    inline void             ResetLocalTimeTo    ( TimeType t );

private:

    void            InitWrapEvaluators  ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

    void            SetWrapEvaluatorPre ( TimelineWrapMethod method );
    void            SetWrapEvaluatorPost( TimelineWrapMethod method );

    inline TimeType GetLocalTimeNoClamp () const;
    inline TimeType GetLocalTimeNoClamp ( TimeType globalTime ) const;

    inline TimeType EvalClamp           ( TimeType t ) const;

    inline TimeType EvalPreClamp        ( TimeType t ) const;
    inline TimeType EvalPostClamp       ( TimeType t ) const;
    inline TimeType EvalRepeat          ( TimeType t ) const;
    inline TimeType EvalMirror          ( TimeType t ) const;

    inline TimeType EvalPre             ( TimeType t ) const;
    inline TimeType EvalPost            ( TimeType t ) const;

};

} //model
} //bv

#include "TimeSegmentEvalImpl.inl"
