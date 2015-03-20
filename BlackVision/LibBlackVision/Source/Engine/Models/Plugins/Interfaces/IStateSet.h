#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IState.h"

namespace bv { namespace model {

class IStateSet;
DEFINE_PTR_TYPE(IStateSet)

class IStateSet
{
public:

    //virtual std::vector< IStatePtr > &      GetStates           () = 0;
    virtual IStatePtr                       GetState            ( const std::string & name ) = 0;

    virtual ~IStateSet(){}

};

} //model
} //bv
