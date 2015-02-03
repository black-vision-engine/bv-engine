#include "BaseEvent.h"

namespace bv
{

// *************************************
//
BaseEvent::BaseEvent                    ( TimeType timeStamp )
    : m_timeStamp( timeStamp )
{
}

// *************************************
//
TimeType    BaseEvent::GetTimeStamp     () const
{
    return m_timeStamp;
}

// *************************************
//
void        BaseEvent::Serialize        ( std::ostringstream & out ) const
{
    { out; } // FIXME: suppress unused warning
    //Default implemenation is empty
}

// *************************************
//
void        BaseEvent::Deserialize      ( std::istringstream & in )
{
    { in; } // FIXME: suppress unused warning
    //Default implemenation is empty
}

} //bv
