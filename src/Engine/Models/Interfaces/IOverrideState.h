#pragma once

namespace bv { 
    
class IValue;

namespace model {

class IParameter;

class IOverrideState
{
public:

    virtual bool                IsAlphaEnabled  ()                          = 0;
    virtual void                DisableAlpha    ()                          = 0;
    virtual void                EnableAlpha     ()                          = 0;

    virtual IParameter *        GetAlphaParam   ()                          = 0;
    virtual IValue *            GetAlphaValue   ()                          = 0;

    virtual void                SetCurAlphaVal  ( const IValue * param )    = 0;
    virtual const IValue *      GetCurAlphaVal  () const                    = 0;

    virtual                     ~IOverrideState () {}

};

} //model
} //bv

