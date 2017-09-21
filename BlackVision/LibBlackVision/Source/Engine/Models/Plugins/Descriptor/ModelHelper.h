#pragma once

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleStateUpdater.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Types/TypeTraits.h"





#define START_MODEL( timeEval ) ModelHelper h( timeEval );

#define END_MODEL() return h.GetModel();

#define ADD_VAC_PARAM( name, defVal ) h.AddVacSimpleParam( name, defVal );
#define ADD_PS_PARAM( name, defVal ) h.AddPsSimpleParam( name, defVal );

#define ADD_VAC_STATED_PARAM( name, defVal ) h.AddVacSimpleStatedParam( name, defVal );

#define ADD_PS_EVAL_PARAM( name, defVal ) h.AddPsSimpleParam( name, defVal, true, false );

namespace bv { namespace model {


template< typename T >
struct ValueParamState
{
	const static ParamType PT = Value2ParamTypeTrait< T >::ParamT;

	typedef typename ValueT< T >::Type																			VType;
	typedef SimpleParameterImpl< CompositeInterpolator< TimeType,
		typename Type2InterpolatorType< T >::Type >,
		typename Type2InterpolatorType< T >::Type,
		ParamType2ModelParamType< PT >::MPT >	PType;
	typedef SimpleState< T >																					SType;

	const VType *				valuePtr;
	const PType *				paramPtr;
	const SimpleState< T > *	statePtr;

	ValueParamState()
		: valuePtr(nullptr)
		, paramPtr(nullptr)
		, statePtr(nullptr)
	{}

	ValueParamState(const VType * value, const PType * param, const SimpleState< T > * state)
		: valuePtr(value)
		, paramPtr(param)
		, statePtr(state)
	{}

	inline const T &		GetValue() const
	{
		assert(valuePtr != nullptr);
		return valuePtr->GetValue();
	}

	inline const PType &	GetParameter() const
	{
		assert(paramPtr != nullptr);
		return *paramPtr;
	}

	inline bool		Changed() const
	{
		assert(statePtr != nullptr);
		return statePtr->SimpleState< T >::StateChanged();
	}
};

template< typename T >
ValueParamState< T >		GetValueParamState(IParamValModel * paramValModel, const std::string & name)
{
	typedef typename ValueParamState< T >::VType	VType;
	typedef typename ValueParamState< T >::PType	PType;

	auto v = paramValModel->GetValue(name);
	auto p = paramValModel->GetParameter(name);

	auto vT = v ? QueryTypedValue< VType >(v.get()) : nullptr;
	auto pT = p ? QueryTypedParam< PType >(p.get()) : nullptr;

	return ValueParamState< T >(vT, pT, static_cast< const SimpleState< T > * >( paramValModel->GetState(name).get() ) );
}


class ModelHelper
{
    DefaultPluginParamValModelPtr           m_model;

    DefaultParamValModelPtr                 m_lastParamValModel;
    ITimeEvaluatorPtr                       m_lastTimeEvaluator;

public:
    ModelHelper ( ITimeEvaluatorPtr te, DefaultPluginParamValModelPtr model );
    ModelHelper ( ITimeEvaluatorPtr te );

    DefaultPluginParamValModelPtr           GetModel                ()  { return m_model; }

    void                                    SetOrCreateVacModel     ();
    void                                    SetOrCreateVSModel      ();
    void                                    SetOrCreatePSModel      ();
    void                                    SetOrCreateGSModel      ();
    void                                    SetOrCreatePluginModel  ();


    inline void                             AddTransformParam   ( std::string name, bool addValue = true ) const
    {
        auto param = std::make_shared< ParamTransform >( name, CompositeTransform(), m_lastTimeEvaluator );
        if( addValue )
        {
            auto evaluator = ParamValEvaluatorFactory::CreateSimpleEvaluator< CompositeTransform, glm::mat4, ModelParamType::MPT_TRANSFORM, ParamType::PT_MAT4, ParamTransform >( param );
            m_lastParamValModel->RegisterAll( evaluator );
        }
        else
        {
            m_lastParamValModel->AddParameter( param );
        }             
    }

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
            evaluator->Evaluate();      // Set Value to defaultValue.
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
        static_assert( false, "You shouldn't be here. Check parameter default value type." );
        assert( false );
    }

    template<>
    inline void                             AddSimpleParam< bool > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const bool& defaultValue, bool addValue, bool isState ) const
    {
        AddParam< BoolInterpolator, bool, ModelParamType::MPT_BOOL, ParamType::PT_BOOL, ParamBool >
            ( model, timeEvaluator, name, defaultValue, addValue, isState );
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

    template<>
    inline void                             AddSimpleParam< std::wstring > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const std::wstring& defaultValue, bool addValue, bool isState ) const
    {
        AddParam< WStringInterpolator, std::wstring, ModelParamType::MPT_WSTRING, ParamType::PT_WSTRING, ParamWString >
            ( model, timeEvaluator, name, defaultValue, addValue, isState );
    }

    template<>
    inline void                             AddSimpleParam< std::string > ( const DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const std::string& defaultValue, bool addValue, bool isState ) const
    {
        AddParam< StringInterpolator, std::string, ModelParamType::MPT_STRING, ParamType::PT_STRING, ParamString >
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

    template< typename ValueType >
    inline void                             AddPsSimpleParam      ( std::string name, const ValueType& defaultValue, bool addValue = false, bool isState = false ) const
    {
        auto model = GetPsModel();
        AddSimpleParam< ValueType >( model, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
    }

    template< typename ValueType >
    inline void                             AddPsSimpleStatedParam      ( std::string name, const ValueType & defaultValue ) const
    {
        auto model = GetPsModel();
        AddSimpleParam< ValueType >( model, m_lastTimeEvaluator, name, defaultValue, true, true );
    }

    template< typename ValueType >
    inline void                             AddValue                    ( std::string name, const ValueType & defaultValue ) const
    {
        m_lastParamValModel->AddValue( std::static_pointer_cast< IValue >( ValuesFactory::CreateValue( name, defaultValue ) ) );
    }

    template< typename ValueType >
    inline void                             AddEnumParam                ( std::string name, const ValueType& defaultValue, bool addValue = false, bool isState = false ) const
    {
        AddParam< IntInterpolator, ValueType, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, model::ParamEnum< ValueType > >
            ( name, defaultValue, addValue, isState );
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
    
    DefaultParamValModelPtr GetPsModel() const
    {
        if( !m_model->GetPixelShaderChannelModel() )
        {
            auto vacModel = std::make_shared< DefaultParamValModel >();
            m_model->SetPixelShaderChannelModel( vacModel );
        }

        return m_model->PixelShaderChannelModelImpl();
    }
};

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"

} //model
} //bv



#define DEFINE_ENUM_PARAMETER_CREATOR( enumName )                       \
namespace bv { namespace model {                                        \
VoidPtr         ParamEnum< enumName >::QueryParamTyped  () \
{                                                                       \
    return std::static_pointer_cast< void >( shared_from_this() );      \
}                                                                       \
template<>                                                              \
static IParameterPtr   ParametersFactory::CreateTypedParameter< enumName >                 ( const std::string & name, ITimeEvaluatorPtr timeline )  \
{                                                                               \
    return CreateParameterEnum< enumName >( name, timeline );           \
}                                                                       \
}}  // model // bv


#define DEFINE_ENUM_PARAMETER_FUNCTIONS( enumName )     \
DEFINE_ENUM_SET_PARAMETER( enumName )                   \
DEFINE_ENUM_PARAMETER_CREATOR( enumName )               



