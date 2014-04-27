#include "System/BasicTypes.h"
#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class TimeSegmentEvalImpl
{
private:

    TimeType    m_duration;

public:

                TimeSegmentEvalImpl ( TimeType duration, TimelinePlayDirection direction = TimelinePlayDirection::TPD_FORWAD, TimelineWrapMethod preMethod = TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod postMethod = TimelineWrapMethod::TWM_CLAMP );
                ~TimeSegmentEvalImpl();
                
    void        Start               ();
    void        Stop                ();
    void        Reverse             ();
    void        Reset               ();

    void        SetGlobalTime       ( TimeType t );
    TimeType    GetLocatTime        () const;

    void        SetWrapPreBehavior  ( TimelineWrapMethod method );
    void        SetWrapPostBehavior ( TimelineWrapMethod method );
    void        SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

private:

    TimeType    GetLocalTimeNoClamp () const;
    TimeType    Clamp               ( TimeType t );

};

} //model
} //bv
