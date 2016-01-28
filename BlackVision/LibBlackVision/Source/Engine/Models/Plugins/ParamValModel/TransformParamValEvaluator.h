#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

class TransformParamValEvaluator;
DEFINE_PTR_TYPE(TransformParamValEvaluator)

class TransformParamValEvaluator : public IParamValEvaluator
{
private:

    ParamTransformPtr       m_param;
    ValueMat4Ptr            m_mat4Values;

    std::vector< IParameterPtr >        m_paramWrapper;
    std::vector< bv::IValueConstPtr >   m_values;

public:

                    TransformParamValEvaluator ( ParamTransformPtr param, const ValueMat4Ptr val );

public:

    virtual std::vector< IParameterPtr > &              GetParameters   () override;
    virtual const std::vector< bv::IValueConstPtr > &   GetValues       () const override;

    virtual IParameterPtr                               GetParameter    ( const std::string & name ) override;
    virtual bv::IValueConstPtr                          GetValue        ( const std::string & name ) const override;

    virtual void                                        Evaluate        () override;

    ParamTransformPtr                                   Parameter       ();
    ValueMat4Ptr &                                      Value           ();

    friend class ParamValEvaluatorFactory;

};

    

} //model
} //bv
