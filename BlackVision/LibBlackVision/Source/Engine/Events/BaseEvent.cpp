#include "stdafx.h"

#include "BaseEvent.h"



#include "Memory/MemoryLeaks.h"



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
void        BaseEvent::Serialize        ( ISerializer& /*ser*/ ) const
{
    //Default implemenation is empty
}


} //bv
