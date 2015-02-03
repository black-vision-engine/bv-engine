#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

template< typename ParamTypePtr, typename ValueTypePtr >
class SimpleParamValEvaluator : public IParamValEvaluator
{
private:

    ParamTypePtr    m_param;
    ValueTypePtr    m_value;

    std::vector< IParameterPtr >        m_paramWrapper;
    std::vector< bv::IValueConstPtr >   m_valueWrapper;

public:

                    SimpleParamValEvaluator ( ParamTypePtr param, ValueTypePtr val );

public:

    virtual std::vector< IParameterPtr > &              GetParameters   () override;
    virtual const std::vector< bv::IValueConstPtr > &   GetValues       () const override;
    
    virtual IParameterPtr                               GetParameter    ( const std::string & name ) override;
    virtual bv::IValueConstPtr                          GetValue        ( const std::string & name ) const override;

    virtual void                                        Evaluate        () override;

    ParamTypePtr                                        Parameter       ();
    ValueTypePtr                                        Value           ();

    friend class ParamValEvaluatorFactory;

};

// Make sure that all types defined here are explicitely instantiated
typedef SimpleParamValEvaluator< ParamFloatPtr, ValueFloatPtr >       SimpleFloatEvaluator;
typedef SimpleParamValEvaluator< ParamVec4Ptr, ValueVec4Ptr >         SimpleVec4Evaluator;
typedef SimpleParamValEvaluator< ParamVec3Ptr, ValueVec3Ptr >         SimpleVec3Evaluator;
typedef SimpleParamValEvaluator< ParamVec2Ptr, ValueVec2Ptr >         SimpleVec2Evaluator;
typedef SimpleParamValEvaluator< ParamMat2Ptr, ValueMat2Ptr >         SimpleMat2Evaluator;
typedef SimpleParamValEvaluator< ParamTransformPtr, ValueMat4Ptr >    SimpleTransformEvaluator;

DEFINE_PTR_TYPE(SimpleFloatEvaluator)
DEFINE_PTR_TYPE(SimpleVec4Evaluator)
DEFINE_PTR_TYPE(SimpleVec3Evaluator)
DEFINE_PTR_TYPE(SimpleVec2Evaluator)
DEFINE_PTR_TYPE(SimpleMat2Evaluator)
DEFINE_PTR_TYPE(SimpleTransformEvaluator)

} //model
} //bv
