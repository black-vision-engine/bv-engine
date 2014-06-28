#pragma once

#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"

#include "Engine/Models/Interfaces/IOverrideState.h"


namespace bv { namespace model {

class BasicOverrideState : public IOverrideState
{
private:

    ParamFloatPtr       m_param;
    ValueFloatPtr       m_value;

    bool                m_enabled;
    bool                m_changed;

public:

                                BasicOverrideState  ( ITimeEvaluatorPtr timeEvaluator );
                                ~BasicOverrideState ();

    virtual void                Update              ( TimeType t ) override;

    virtual bool                Changed             () const override;
    void                        SetChanged          ( bool changed );

    virtual bool                IsAlphaEnabled      () const override;

    virtual void                DisableAlpha        () override;
    virtual void                EnableAlpha         () override;

    virtual IParameterPtr       GetAlphaParam       () override;
    virtual IValueConstPtr      GetAlphaValue       () const override;

};

} //model
} //bv
