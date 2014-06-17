#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

class TransformVecParamValEvaluator;
DEFINE_PTR_TYPE(TransformVecParamValEvaluator)

class TransformVecParamValEvaluator : public IParamValEvaluator
{
private:

    ParamTransformVecPtr    m_param;
    ValueMat4PtrVec         m_mat4Values;

    std::vector< IParameterPtr >        m_paramWrapper;
    std::vector< bv::IValueConstPtr >   m_values;

public:

                    TransformVecParamValEvaluator ( ParamTransformVecPtr param, const ValueMat4PtrVec val );

public:

    virtual std::vector< IParameterPtr > &              GetParameters   () override;
    virtual const std::vector< bv::IValueConstPtr > &   GetValues       () const override;

    virtual IParameterPtr                               GetParameter    ( const std::string & name ) override;
    virtual bv::IValueConstPtr                          GetValue        ( const std::string & name ) const override;

    virtual void                                        Evaluate        () override;

    ParamTransformVecPtr                                Parameter       ();
    ValueMat4PtrVec &                                   Value           ();

    friend class ParamValEvaluatorFactory;

};

    

} //model
} //bv
