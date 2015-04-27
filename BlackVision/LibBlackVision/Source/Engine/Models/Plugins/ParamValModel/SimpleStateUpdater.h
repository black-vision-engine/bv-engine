#pragma once

#include "Engine/Interfaces/IUpdater.h"

#include "Engine/Models/Plugins/Interfaces/IState.h"
#include "SimpleTypedStates.h"
#include "Engine/Types/Values/TypedValues.h"

namespace bv { namespace model {

template< typename StateTypePtr, typename ValueTypePtr >
class SimpleStateUpdater : public IUpdater
{
    StateTypePtr                            m_state;
    ValueTypePtr                            m_value;

public:

    SimpleStateUpdater( StateTypePtr state, ValueTypePtr value );

    virtual void    DoUpdate    ();
};

typedef SimpleStateUpdater< FloatSimpleStatePtr, ValueFloatConstPtr > SimpleFloatStateUpdater;

} //model
} //bv

#include "SimpleStateUpdater.inl"