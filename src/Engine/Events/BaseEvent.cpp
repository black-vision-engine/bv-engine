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
    //Default implemenation is empty
}

// *************************************
//
void        BaseEvent::Deserialize      ( std::istringstream & in )
{
    //Default implemenation is empty
}

} //bv
