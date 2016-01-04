#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

class ModelNodeEffectBase : public IModelNodeEffect
{
protected:

    DefaultParamValModelPtr m_paramValModel;

public:

                            ModelNodeEffectBase     ( ITimeEvaluatorPtr timeEvaluator );

    virtual void            Serialize               ( ISerializer& ser ) const;

    virtual void            Update                  ( TimeType t ) override;

    IParameterPtr                           GetParameter    ( const std::string & name ) const override;
    const std::vector< IParameterPtr > &    GetParameters   () const override;

    const std::vector< IValueConstPtr > &   GetValues       () const override;

};

} // model
} // bv
