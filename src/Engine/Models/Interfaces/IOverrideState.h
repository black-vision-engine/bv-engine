#pragma once

#include "Engine/Interfaces/IUpdatable.h"


namespace bv { 
    
class IValue;

namespace model {

class IParameter;

class IOverrideState : public bv::IUpdatable
{
public:

    virtual bool                IsOverriden     () const                = 0;
    virtual bool                IsEnabled       () const                = 0;
    
    virtual void                Disable         ()                      = 0;
    virtual void                Enable          ()                      = 0;

    virtual bool                IsAlphaEnabled  () const                = 0;
    virtual bool                IsAlphaOverriden() const                = 0;

    virtual void                DisableAlpha    ()                      = 0;
    virtual void                EnableAlpha     ()                      = 0;

    virtual IParameter *        GetAlphaParam   ()                      = 0;
    virtual const IValue *      GetAlphaValue   () const                = 0;

    virtual void                SetCurAlphaVal  ( const IValue * val )  = 0;
    virtual const IValue *      GetCurAlphaVal  () const                = 0;

    virtual                     ~IOverrideState () {}

};

} //model
} //bv

