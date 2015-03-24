#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleStateUpdater.h"
#include "Engine/Interfaces/IValue.h"

namespace bv { namespace model {

class BasePluginDescriptor : public IPluginDescriptor
{
private:

    std::string     m_uid;
    std::string     m_defaultName;

protected:

    BasePluginDescriptor    ( const std::string & uid, const std::string & defaultName );

public:

    virtual const std::string &             GetPluginTypeUID    () const override;
    virtual const std::string &             DefaultPluginName   () const override;

    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;
    virtual IPluginParamValModelPtr         CreateModel         ( ITimeEvaluatorPtr timeEvaluator ) const override;

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const = 0;

    template< typename PluginType >
    std::shared_ptr< PluginType >           CreatePluginTyped    ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator  ) const
    {
        return std::make_shared< PluginType >( name, GetPluginTypeUID(), prev, DefaultPluginParamValModelPtr( CreateDefaultModel( timeEvaluator ) ) );
    }

    virtual ~BasePluginDescriptor () {}

protected:
    DefaultParamValModelPtr                  CreateVacModel      ( DefaultPluginParamValModelPtr&, ITimeEvaluatorPtr ) const;
    
    template< typename T >
    void                                     AddParam            ( DefaultParamValModelPtr&, ITimeEvaluatorPtr, std::string, const T&, bool = false, bool = false ) const
    {
        assert( false );
    }

    template<>
    void                                     AddParam< float >   ( DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const float& defaultValue, bool addValue, bool isState ) const
    {
        auto param = ParametersFactory::CreateParameterFloat( name, timeEvaluator );
        //model->AddParameter( param );
        param->SetVal( defaultValue, 0.f );

        if( addValue )
        {
            auto evaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( param );
            model->RegisterAll( evaluator );
        }

        if( isState )
        {
            assert( addValue );
            auto state = std::make_shared< FloatSimpleState >();
            auto value = model->GetValue( name );
            auto qValue = QueryTypedValue< ValueFloatPtr >( value );
            auto updater = std::make_shared< SimpleFloatStateUpdater >( state, qValue );
            model->AddState( name, state, updater );
        }
    }

};

} //model
} //bv
