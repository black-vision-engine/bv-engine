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


        template< typename InterpolatorType, typename ValueType, ModelParamType MPT, ParamType PT, typename ParamImpl >
        void                                    AddParam            ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const ValueType& defaultValue, bool addValue, bool isState ) const
        {
            assert( model );
            assert( timeEvaluator );
            auto param = std::make_shared< ParamImpl >( name, InterpolatorType(), timeEvaluator );
            param->SetVal( defaultValue, 0.f );

            if( addValue )
            {
                auto evaluator = ParamValEvaluatorFactory::CreateSimpleEvaluator< InterpolatorType, ValueType, MPT, PT >( param );
                model->RegisterAll( evaluator );
            }
            else
            {
                model->AddParameter( param );
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

        template< typename InterpolatorType, typename ValueType, ModelParamType MPT, ParamType PT, typename ParamImpl >
        void                                    AddParam            ( std::string name, const ValueType& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< InterpolatorType, ValueType, MPT, PT, ParamImpl >( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template< typename ValueType >
        inline void                             AddSimpleParam      ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string, const ValueType&, bool = false, bool = false ) const
        {
            assert( false );
        }

        template<>
        inline void                             AddSimpleParam< float > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const float& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< FloatInterpolator, float, ModelParamType::MPT_FLOAT, ParamType::PT_FLOAT1, ParamFloat >
                ( model, timeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< int > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const int& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< IntInterpolator, int, ModelParamType::MPT_INT, ParamType::PT_INT, ParamInt >
                ( model, timeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< glm::vec3 > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const glm::vec3& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3, ParamType::PT_FLOAT3, ParamVec3 >
                ( model, timeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< glm::vec2 > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const glm::vec2& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< Vec2Interpolator, glm::vec2, ModelParamType::MPT_VEC2, ParamType::PT_FLOAT2, ParamVec2 >
                ( model, timeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< glm::vec4 > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const glm::vec4& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4, ParamType::PT_FLOAT4, ParamVec4 >
                ( model, timeEvaluator, name, defaultValue, addValue, isState );
        }

        template< typename ValueType >
        inline void                             AddSimpleParam      ( std::string name, const ValueType& defaultValue, bool addValue = false, bool isState = false ) const
        {
            AddSimpleParam< ValueType >( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template< typename ValueType >
        inline void                             AddSimpleStatedParam      ( std::string name, const ValueType& defaultValue ) const
        {
            AddSimpleParam< ValueType >( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, true, true );
        }

        template< typename ValueType >
        inline void                             AddVacSimpleParam      ( std::string name, const ValueType& defaultValue, bool addValue = false, bool isState = false ) const
        {
            auto model = GetVacModel();
            AddSimpleParam< ValueType >( model, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template< typename ValueType >
        inline void                             AddVacSimpleStatedParam      ( std::string name, const ValueType& defaultValue ) const
        {
            auto model = GetVacModel();
            AddSimpleParam< ValueType >( model, m_lastTimeEvaluator, name, defaultValue, true, true );
        }

private:
        DefaultParamValModelPtr GetVacModel() const
        {
            if( !m_model->GetVertexAttributesChannelModel() )
            {
                auto vacModel = std::make_shared< DefaultParamValModel >();
                m_model->SetVertexAttributesChannelModel( vacModel );
            }

            return m_model->VertexAttributesChannelModelImpl();
        }
    };
    
};

} //model
} //bv

// wget --recursive --no-clobber --page-requisites --html-extension --convert-links --restrict-file-names=windows --domains blender-mirror.kino3d.org --no-parent http://blender-mirror.kino3d.org/peach/