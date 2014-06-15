#pragma once

#include "Engine/Interfaces/IUpdatable.h"


namespace bv { 
    
class IValue;

namespace model {

class IParameter;

class IOverrideState : public bv::IUpdatable
{
public:

    virtual bool                Changed         () const                = 0;

    virtual bool                IsOverriden     () const                = 0;
    virtual bool                IsEnabled       () const                = 0;
    
    virtual void                Disable         ()                      = 0;
    virtual void                Enable          ()                      = 0;

    virtual bool                IsAlphaEnabled  () const                = 0;
    virtual bool                IsAlphaOverriden() const                = 0;

    virtual void                DisableAlpha    ()                      = 0;
    virtual void                EnableAlpha     ()                      = 0;

    virtual IParameterPtr       GetAlphaParam   ()                      = 0;
    virtual IValueConstPtr      GetAlphaValue   () const                = 0;

    virtual void                SetCurAlphaVal  ( IValueConstPtr val )  = 0;
    virtual IValueConstPtr      GetCurAlphaVal  () const                = 0;

    virtual                     ~IOverrideState () {}

};

} //model
} //bv

