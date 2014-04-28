#include <vector>
#include <functional>

#include "System/BasicTypes.h"
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

    TimeType                m_globalTime;

    TimeType                m_duration;
    TimeType                m_pauseDuration;
    TimeType                m_startTime;
    TimeType                m_backwardStartTime;

    TimelinePlayDirection   m_playDirection;

    TimelineWrapMethod      m_wrapPreBehavior;
    TimelineWrapMethod      m_wrapPostBehavior;

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

    void        UpdateGlobalTime    ( TimeType t );
    TimeType    GetLocalTime        () const;

    void        SetWrapBehaviorPre  ( TimelineWrapMethod method );
    void        SetWrapBehaviorPost ( TimelineWrapMethod method );
    void        SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

private:

    void            InitWrapEvaluators  ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

    void            SetWrapEvaluatorPre ( TimelineWrapMethod method );
    void            SetWrapEvaluatorPost( TimelineWrapMethod method );

    inline void     ResetLocalTimeTo    ( TimeType t );

    inline TimeType GetLocalTimeNoClamp () const;

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
