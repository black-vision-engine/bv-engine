#include <vector>
#include <functional>

#include "System/BasicTypes.h"
#include "Mathematics/Core/mathfuncs.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class TimeSegmentEvalImpl
{
private:

    typedef std::function<bv::TimeType(bv::TimeType)> WrapEvaluator;
    typedef std::vector< std::function<bv::TimeType(bv::TimeType)> > WrapEvaluators;

private:

    bool                        m_started;
    bool                        m_paused;

    bv::TimeType                m_globalTime;

    bv::TimeType                m_duration;
    bv::TimeType                m_pauseDuration;
    bv::TimeType                m_startTime;
    bv::TimeType                m_backwardStartTime;

    bv::TimelinePlayDirection   m_playDirection;

    TimelineWrapMethod      m_wrapPreBehavior;
    TimelineWrapMethod      m_wrapPostBehavior;

    WrapEvaluators          m_wrapEvaluatorsPre;
    WrapEvaluators          m_wrapEvaluatorsPost;

    WrapEvaluator           m_wrapEvaluatorPre;
    WrapEvaluator           m_wrapEvaluatorPost;

public:

                TimeSegmentEvalImpl ( bv::TimeType duration, bv::TimelinePlayDirection direction = TimelinePlayDirection::TPD_FORWAD, TimelineWrapMethod preMethod = TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod postMethod = TimelineWrapMethod::TWM_CLAMP );
                ~TimeSegmentEvalImpl();
                
    void        Start               ();
    void        Stop                ();
    void        Reverse             ();
    void        Reset               ( TimelinePlayDirection direction = TimelinePlayDirection::TPD_FORWAD );

    void        UpdateGlobalTime    ( bv::TimeType t );
    bv::TimeType    GetLocalTime        () const;

    void        SetWrapBehaviorPre  ( TimelineWrapMethod method );
    void        SetWrapBehaviorPost ( TimelineWrapMethod method );
    void        SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

private:

    void            InitWrapEvaluators  ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

    void            SetWrapEvaluatorPre ( TimelineWrapMethod method );
    void            SetWrapEvaluatorPost( TimelineWrapMethod method );

    inline bv::TimeType ResetLocalTimeTo    ( bv::TimeType t );

    inline bv::TimeType GetLocalTimeNoClamp () const;

    inline bv::TimeType EvalClamp           ( bv::TimeType t ) const;

    inline bv::TimeType EvalPreClamp        ( bv::TimeType t ) const;
    inline bv::TimeType EvalPostClamp       ( bv::TimeType t ) const;
    inline bv::TimeType EvalRepeat          ( bv::TimeType t ) const;
    inline bv::TimeType EvalMirror          ( bv::TimeType t ) const;

    inline bv::TimeType EvalPre             ( bv::TimeType t ) const;
    inline bv::TimeType EvalPost            ( bv::TimeType t ) const;

};

} //model
} //bv

#include "TimeSegmentEvalImpl.inl"
