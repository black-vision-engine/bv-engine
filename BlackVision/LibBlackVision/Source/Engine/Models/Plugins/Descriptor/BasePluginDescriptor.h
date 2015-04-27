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

    class ModelHelper
    {
        DefaultPluginParamValModelPtr           m_model;

        DefaultParamValModelPtr                 m_lastParamValModel;
        ITimeEvaluatorPtr                       m_lastTimeEvaluator;

    public:
        ModelHelper(  ITimeEvaluatorPtr te ) : m_lastTimeEvaluator( te ) { m_model = std::make_shared< DefaultPluginParamValModel >(); }
        DefaultPluginParamValModelPtr           GetModel() { return m_model; }

        void                                    CreateVacModel      ();


        template< typename ValueType >
        inline void                             AddSimpleParam      ( std::string, const ValueType&, bool = false, bool = false ) const
        {
            assert( false );
        }

#include "ModelHelper.inl"

        template< typename InterpolatorType, typename ValueType, ModelParamType MPT, ParamType PT, typename ParamImpl >
        void                                    AddParam            ( std::string name, const ValueType& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< InterpolatorType, ValueType, MPT, PT, ParamImpl >( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template< typename InterpolatorType, typename ValueType, ModelParamType MPT, ParamType PT, typename ParamImpl >
        void                                    AddParam            ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const ValueType& defaultValue, bool addValue, bool isState ) const
        {
            assert( model );
            assert( timeEvaluator );
            auto param = std::make_shared< ParamImpl >( name, InterpolatorType(), timeEvaluator );
            param->SetVal( defaultValue, 0.f );

            model->AddParameter( param );

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
                auto qValue = QueryTypedValue< std::shared_ptr< ValueImpl< ValueType, PT > > >( value );
                auto updater = std::make_shared< 
                    SimpleStateUpdater< 
                        std::shared_ptr< SimpleState<ValueType> >, 
                        std::shared_ptr< ValueImpl< ValueType, PT > >
                    > >( state, qValue );
                model->AddState( name, state, updater );
            }
        }
    };

    
};

} //model
} //bv

// wget --recursive --no-clobber --page-requisites --html-extension --convert-links --restrict-file-names=windows --domains blender-mirror.kino3d.org --no-parent http://blender-mirror.kino3d.org/peach/