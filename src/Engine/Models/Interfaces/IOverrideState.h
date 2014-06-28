#pragma once

#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/IValue.h"

namespace bv { 
    
class IValue;

namespace model {

class IParameter;

class IOverrideState : public bv::IUpdatable
{
public:

    virtual bool                Changed         () const                = 0;
    
    virtual bool                IsAlphaEnabled  () const                = 0;

    virtual void                DisableAlpha    ()                      = 0;
    virtual void                EnableAlpha     ()                      = 0;

    virtual IParameterPtr       GetAlphaParam   ()                      = 0;
    virtual IValueConstPtr      GetAlphaValue   () const                = 0;

    virtual                     ~IOverrideState () {}

};

} //model
} //bv

