#pragma once

#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"

#include "Engine/Models/Interfaces/IOverrideState.h"


namespace bv { namespace model {

class BasicOverrideState : public IOverrideState
{
private:

    ParamFloat *        m_param;
    ValueFloat *        m_value;

    const ValueFloat *  m_curVal;

public:

                                BasicOverrideState  ( ITimeEvaluatorPtr timeEvaluator );
                                ~BasicOverrideState ();

    virtual bool                IsEnabled           () const override;
    virtual void                Disable             () override;
    virtual void                Enable              () override;

    virtual bool                IsAlphaEnabled      () const override;
    virtual void                DisableAlpha        () override;
    virtual void                EnableAlpha         () override;
                                                       
    virtual IParameter *        GetAlphaParam       () override;
    virtual IValue *            GetAlphaValue       () override;

    virtual void                SetCurAlphaVal      ( const IValue * val ) override;
    virtual const IValue *      GetCurAlphaVal      () const override;

};

} //model
} //bv
