#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleStateUpdater.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

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
    
    template< typename InterpolatorType, typename ValueType, ModelParamType MPT, ParamType PT, typename ParamImpl >
    inline void                                     AddParam            ( DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const ValueType& defaultValue, bool addValue = false, bool isState = false ) const
    {
        //auto param = std::make_shared< SimpleParameterImpl< InterpolatorType, ValueType, MPT > >( name, InterpolatorType(), timeEvaluator );
        auto param = std::make_shared< ParamImpl >( name, InterpolatorType(), timeEvaluator );
        param->SetVal( defaultValue, 0.f );
        //auto param = ParametersFactory::CreateTypedParameter<ValueType>( name, timeEvaluator );
        //SetParameter( param, 0.f, defaultValue );

        if( addValue )
        {
            auto evaluator = ParamValEvaluatorFactory::CreateSimpleEvaluator< InterpolatorType, ValueType, MPT, PT >( param );
            model->RegisterAll( evaluator );
        }

        if( isState )
        {
            assert( addValue );
            auto state = std::make_shared< SimpleState<ValueType> >();
            auto value = model->GetValue( name );
            auto qValue = QueryTypedValue< std::shared_ptr< ValueImpl< ValueType, PT > > >( value ); // FIXME
            auto updater = std::make_shared< 
                SimpleStateUpdater< 
                    std::shared_ptr< SimpleState<ValueType> >, 
                    std::shared_ptr< ValueImpl< ValueType, PT > >   // FIXME
                > >( state, qValue );
            model->AddState( name, state, updater );
        }
    }
};

//template< typename T >
//inline void                                     BasePluginDescriptor::AddParam< T >            ( DefaultParamValModelPtr&, ITimeEvaluatorPtr, std::string, const T&, bool, bool ) const
//{
//    assert( false );
//}

//template<>
//inline void                                     BasePluginDescriptor::AddParam< float >   ( DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const float& defaultValue, bool addValue, bool isState ) const
//{
//    auto param = ParametersFactory::CreateParameterFloat( name, timeEvaluator );
//    //model->AddParameter( param );
//    param->SetVal( defaultValue, 0.f );
//
//    if( addValue )
//    {
//        auto evaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( param );
//        model->RegisterAll( evaluator );
//    }
//
//    if( isState )
//    {
//        assert( addValue );
//        auto state = std::make_shared< FloatSimpleState >();
//        auto value = model->GetValue( name );
//        auto qValue = QueryTypedValue< ValueFloatPtr >( value );
//        auto updater = std::make_shared< SimpleFloatStateUpdater >( state, qValue );
//        model->AddState( name, state, updater );
//    }
//}

} //model
} //bv
