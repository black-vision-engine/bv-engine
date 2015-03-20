#pragma once

#include "System/BasicTypes.h"

namespace bv { namespace model {

class IState
{
public:
    virtual bool StateChanged() const = 0;
};

DEFINE_PTR_TYPE(IState)

//template< typename T >
//class SimpleState : public IStateValue {
//    T prevVal, curVal;
//public:
//    void Update( const T& val )
//    {
//        prevVal = curVal;
//        curVal = val;
//    }
//    virtual bool StateChanged() const override
//    {
//        return prevVal != curVal;
//    }
//    const T& GetValue() const
//    {
//        return curVal;
//    }
//};


} //model
} //bv
 