#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"


namespace bv { namespace model {

class DefaultParamValModel : public IParamValModel
{
private:

    std::vector< IParameter * >             m_parameters;
    std::vector< bv::IValue * >             m_values;
    std::vector< IParamValEvaluator * >     m_evaluators;

public:

            DefaultParamValModel            ();
            ~DefaultParamValModel           ();
                                        
    virtual std::vector< IParameter * > &           GetParameters   () override;
    virtual const std::vector< bv::IValue * > &     GetValues       () const override;
    virtual std::vector< IParamValEvaluator * > &   GetEvaluators   () override;

    virtual IParameter *                            GetParameter    ( const std::string & name ) override;
    virtual bv::IValue *                            GetValue        ( const std::string & name ) const override;

    virtual void                                    Update          ( TimeType t ) override;

    void                                            AddParameter    ( IParameter * param );
    void                                            AddValue        ( bv::IValue * val );
    void                                            AddEvaluator    ( IParamValEvaluator * evaluator );
    void                                            RegisterAll     ( IParamValEvaluator * evaluator );

};

} //model
} //bv
