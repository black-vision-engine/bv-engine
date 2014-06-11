#pragma once

namespace bv { 
    
class IValue;

namespace model {

class IParameter;

class IOverrideState
{
public:

    virtual bool                IsEnabled       () const                = 0;
    virtual void                Disable         ()                      = 0;
    virtual void                Enable          ()                      = 0;

    virtual bool                IsAlphaEnabled  () const                = 0;
    virtual void                DisableAlpha    ()                      = 0;
    virtual void                EnableAlpha     ()                      = 0;

    virtual IParameter *        GetAlphaParam   ()                      = 0;
    virtual IValue *            GetAlphaValue   ()                      = 0;

    virtual void                SetCurAlphaVal  ( const IValue * val )  = 0;
    virtual const IValue *      GetCurAlphaVal  () const                = 0;

    virtual                     ~IOverrideState () {}

};

} //model
} //bv

