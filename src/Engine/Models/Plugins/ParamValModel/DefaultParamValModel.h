#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"


namespace bv { namespace model {

class DefaultParamValModel : public IParamValModel
{
private:

    std::vector< IParameter * >             m_parameters;
    std::vector< IValue * >                 m_values;
    std::vector< IParamValEvaluator * >     m_evaluators;

public:

            DefaultParamValModel            ();
            ~DefaultParamValModel           ();
                                        
    virtual std::vector< IParameter * > &           GetParameters   () override;
    virtual std::vector< IValue * > &               GetValues       () override;
    virtual std::vector< IParamValEvaluator * > &   GetEvaluators   () override;

    virtual void                                    Update          ( TimeType t ) override;

    void                                            AddParameter    ( IParameter * param );
    void                                            AddValue        ( IValue * val );
    void                                            AddEvaluator    ( IParamValEvaluator * evaluator );

};

} //model
} //bv
