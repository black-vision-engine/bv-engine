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

    bool                m_enabledAM;
    bool                m_enabledNM;
    bool                m_changedAM;
    bool                m_changedNM;

public:

                                BasicOverrideState  ( ITimeEvaluatorPtr timeEvaluator );
                                ~BasicOverrideState ();

    virtual void                Update              ( TimeType t ) override;

    virtual bool                ChangedAM           () const override;
    virtual bool                ChangedNM           () const override;

    void                        SetChangedAM        ( bool changed );
    void                        SetChangedNM        ( bool changed );

    virtual bool                IsAlphaEnabledAM    () const override;
    virtual bool                IsAlphaEnabledNM    () const override;

    virtual void                EnableAlphaAM       () override;
    virtual void                EnableAlphaNM       () override;

    virtual void                DisableAlphaAM      () override;
    virtual void                DisableAlphaNM      () override;

    virtual IParameterPtr       GetAlphaParam       () override;
    virtual IValueConstPtr      GetAlphaValue       () const override;

};

} //model
} //bv
