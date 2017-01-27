#pragma once

#include "Engine/Models/Plugins/Interfaces/IState.h"

namespace bv { namespace model {

template< typename T >
class SimpleState final : public IStatedValue {
    T prevVal, curVal;
public:
	inline void Update( const T& val )
    {
        prevVal = curVal;
        curVal = val;
    }
    inline bool StateChanged() const override
    {
        return prevVal != curVal;
    }
	inline const T& GetValue() const
    {
        return curVal;
    }
};

typedef SimpleState<float> FloatSimpleState;

DEFINE_PTR_TYPE( FloatSimpleState );

} //model
} //bv
