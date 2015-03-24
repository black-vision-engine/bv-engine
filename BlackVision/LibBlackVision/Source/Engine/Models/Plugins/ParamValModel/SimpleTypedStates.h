#pragma once

#include "Engine/Models/Plugins/Interfaces/IState.h"

namespace bv { namespace model {

template< typename T >
class SimpleState : public IState {
    T prevVal, curVal;
public:
    void Update( const T& val )
    {
        prevVal = curVal;
        curVal = val;
    }
    virtual bool StateChanged() const override
    {
        return prevVal != curVal;
    }
    const T& GetValue() const
    {
        return curVal;
    }
};

typedef SimpleState<float> FloatSimpleState;

DEFINE_PTR_TYPE( FloatSimpleState );

} //model
} //bv
