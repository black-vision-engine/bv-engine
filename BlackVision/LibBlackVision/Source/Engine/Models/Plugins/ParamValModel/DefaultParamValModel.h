#pragma once

#include <vector>
#include <map>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Interfaces/IParamValEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IState.h"
#include "Engine/Interfaces/IUpdater.h"

namespace bv { namespace model {

class DefaultParamValModel;
DEFINE_PTR_TYPE( DefaultParamValModel )

class DefaultParamValModel : public IParamValModel
{
private:

    std::vector< IParameterPtr >            m_parameters;

    std::vector< IParamValEvaluatorPtr >    m_evaluators;
    std::vector< bv::IValueConstPtr >       m_values;

    std::vector< IStateUpdaterPtr >         m_stateUpdaters;
    std::map< std::string, IStatedValuePtr > m_states;

    std::vector< bv::IValuePtr >            m_valuesNC;

public:

            DefaultParamValModel            ();
            ~DefaultParamValModel           ();

    virtual void                                        Serialize       ( ISerializer& ser ) const;

    virtual std::vector< IParameterPtr > &              GetParameters   () override;
    virtual const std::vector< bv::IValueConstPtr > &   GetValues       () const override;
    virtual std::vector< IParamValEvaluatorPtr > &      GetEvaluators   () override;

    virtual const std::map< std::string, IStatedValuePtr > &    GetStates   () override;

    virtual IParameterPtr                               GetParameter    ( const std::string & name ) override;
    virtual bv::IValueConstPtr                          GetValue        ( const std::string & name ) const override;
    virtual IStatedValuePtr                             GetState        ( const std::string & name ) override;

    virtual void                                        Update          () override;

    void                                                RemoveParamVal  ( const std::string & name );

    void                                                SetParameter    ( IParameterPtr param );
    void                                                AddParameter    ( IParameterPtr param );
    void                                                AddValue        ( bv::IValuePtr val );
    void                                                AddValue        ( bv::IValueConstPtr val );
    void                                                AddEvaluator    ( IParamValEvaluatorPtr evaluator );
    void                                                RegisterAll     ( IParamValEvaluatorPtr evaluator );
    void                                                AddState        ( const std::string & name, IStatedValuePtr state, IStateUpdaterPtr updater );

    const std::vector< bv::IValuePtr > &                GetValuesNC     () const;

    static DefaultParamValModelPtr                      Create          ();
};

} //model
} //bv
