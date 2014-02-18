#pragma once

#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"

#include "Engine/Models/Plugins/Parameters/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

class TransformVecParamValEvaluator : public IParamValEvaluator
{
private:

    ParamTransformVec *     m_param;
    ValueMat4PtrVec         m_mat4Values;

    std::vector< IParameter * >     m_paramWrapper;
    std::vector< IValue * >         m_values;

private:

                    TransformVecParamValEvaluator ( ParamTransformVec * param, const ValueMat4PtrVec val );

public:

    virtual std::vector< IParameter * > &   GetParameters   () override;
    virtual const std::vector< IValue * > & GetValues       () const override;

    virtual IParameter *                    GetParameter    ( const std::string & name ) override;
    virtual IValue *                        GetValue        ( const std::string & name ) const override;

    virtual void                            Evaluate        ( TimeType t ) override;

    ParamTransformVec *                     Param           ();
    ValueMat4PtrVec &                       Value           ();

    friend class ParamValEvaluatorFactory;

};

    

} //model
} //bv
