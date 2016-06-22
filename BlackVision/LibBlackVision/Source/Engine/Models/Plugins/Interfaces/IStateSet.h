#pragma once

#include <vector>
#include <map>

#include "Engine/Models/Plugins/Interfaces/IState.h"

namespace bv { namespace model {

class IStateSet;
DEFINE_PTR_TYPE(IStateSet)

class IStateSet
{
public:

    //virtual std::vector< IStatedValuePtr > &      GetStates           () = 0;
    virtual IStatedValuePtr                                     GetState        ( const std::string & name ) = 0;
    virtual const std::map< std::string, IStatedValuePtr > &    GetStates       () = 0;

    virtual ~IStateSet(){}

};

} //model
} //bv
