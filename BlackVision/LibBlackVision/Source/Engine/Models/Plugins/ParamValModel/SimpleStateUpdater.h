#pragma once

#include "Engine/Interfaces/IUpdater.h"

#include "Engine/Models/Plugins/Interfaces/IState.h"
#include "SimpleTypedStates.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv { namespace model {



template< typename StateTypePtr, typename ValueTypePtr >
class SimpleStateUpdater : public IStateUpdater
{
private:

    StateTypePtr                            m_state;
    ValueTypePtr                            m_value;

public:

    SimpleStateUpdater( StateTypePtr state, ValueTypePtr value );

    virtual void                    DoUpdate    () override;
    virtual const std::string &     GetName     () override;

};

typedef SimpleStateUpdater< FloatSimpleStatePtr, ValueFloatConstPtr > SimpleFloatStateUpdater;

} //model
} //bv

#include "SimpleStateUpdater.inl"
