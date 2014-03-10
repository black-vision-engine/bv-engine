#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"


namespace bv { namespace model {

class DefaultParamValModel : public IParamValModel
{
private:

    std::vector< IParameter * >             m_parameters;
    std::vector< const bv::IValue * >       m_values;
    std::vector< bv::IValue * >             m_valuesNC;
    std::vector< IParamValEvaluator * >     m_evaluators;

public:

            DefaultParamValModel            ();
            ~DefaultParamValModel           ();
                                        
    virtual std::vector< IParameter * > &               GetParameters   () override;
    virtual const std::vector< const bv::IValue * > &   GetValues       () const override;
    virtual std::vector< IParamValEvaluator * > &       GetEvaluators   () override;

    virtual IParameter *                                GetParameter    ( const std::string & name ) override;
    virtual const bv::IValue *                          GetValue        ( const std::string & name ) const override;

    virtual void                                        Update          ( TimeType t ) override;

    void                                                AddParameter    ( IParameter * param );
    void                                                AddValue        ( bv::IValue * val );
    void                                                AddValue        ( const bv::IValue * val );
    void                                                AddEvaluator    ( IParamValEvaluator * evaluator );
    void                                                RegisterAll     ( IParamValEvaluator * evaluator );

    const std::vector< bv::IValue * > &                 GetValuesNC     () const;

};

} //model
} //bv
