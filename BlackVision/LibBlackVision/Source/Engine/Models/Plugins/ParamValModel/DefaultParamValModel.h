#pragma once

#include <vector>
#include <map>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IState.h"
#include "Engine/Interfaces/IUpdater.h"

namespace bv { namespace model {

class DefaultParamValModel : public IParamValModel
{
private:

    std::vector< IParameterPtr >            m_parameters;

    std::vector< IParamValEvaluatorPtr >    m_evaluators;
    std::vector< bv::IValueConstPtr >       m_values;

    std::vector< IUpdaterPtr >              m_stateUpdaters;
    std::map< std::string, IStatePtr >      m_states;

    std::vector< bv::IValuePtr >            m_valuesNC;

public:

            DefaultParamValModel            ();
            ~DefaultParamValModel           ();
                                        
    virtual std::vector< IParameterPtr > &              GetParameters   () override;
    virtual const std::vector< bv::IValueConstPtr > &   GetValues       () const override;
    virtual std::vector< IParamValEvaluatorPtr > &      GetEvaluators   () override;

    virtual IParameterPtr                               GetParameter    ( const std::string & name ) override;
    virtual bv::IValueConstPtr                          GetValue        ( const std::string & name ) const override;
    virtual IStatePtr                                   GetState        ( const std::string & name ) override;

    virtual void                                        Update          () override;

    void                                                AddParameter    ( IParameterPtr param );
    void                                                AddValue        ( bv::IValuePtr val );
    void                                                AddValue        ( bv::IValueConstPtr val );
    void                                                AddEvaluator    ( IParamValEvaluatorPtr evaluator );
    void                                                RegisterAll     ( IParamValEvaluatorPtr evaluator );
    void                                                AddState        ( const std::string & name, IStatePtr state, IUpdaterPtr updater );

    const std::vector< bv::IValuePtr > &                GetValuesNC     () const;

};

DEFINE_PTR_TYPE(DefaultParamValModel)

} //model
} //bv
