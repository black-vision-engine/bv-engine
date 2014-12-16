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

    virtual bool                ChangedAM       () const                = 0;
    virtual bool                ChangedNM       () const                = 0;
    
    virtual bool                IsAlphaEnabledAM() const                = 0;
    virtual bool                IsAlphaEnabledNM() const                = 0;

    virtual void                EnableAlphaAM   ()                      = 0;
    virtual void                EnableAlphaNM   ()                      = 0;
    virtual void                DisableAlphaAM  ()                      = 0;
    virtual void                DisableAlphaNM  ()                      = 0;

    virtual IParameterPtr       GetAlphaParam   ()                      = 0;
    virtual IValueConstPtr      GetAlphaValue   () const                = 0;

    virtual                     ~IOverrideState () {}

};

} //model
} //bv

