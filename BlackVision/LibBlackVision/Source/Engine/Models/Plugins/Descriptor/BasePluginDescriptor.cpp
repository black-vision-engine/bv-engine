#include "BasePluginDescriptor.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleStateUpdater.h"

namespace bv { namespace model {

// *********************************
//
BasePluginDescriptor::BasePluginDescriptor                      ( const std::string & uid, const std::string & defaultName )
    : m_uid( uid )
    , m_defaultName( defaultName )
{
}

// *********************************
//
const std::string &     BasePluginDescriptor::GetPluginTypeUID  () const
{
    return m_uid;
}

// *********************************
//
const std::string &     BasePluginDescriptor::DefaultPluginName () const
{
    return m_defaultName;
}

// *********************************
//
bool                    BasePluginDescriptor::CanBeAttachedTo   ( IPluginConstPtr plugin )  const
{
    //By default assume that plugin can be connected just to any other plugin
    return true;
}

// *********************************
//
IPluginParamValModelPtr BasePluginDescriptor::CreateModel       ( ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreateDefaultModel( timeEvaluator );
}

// *********************************
//
DefaultParamValModelPtr                  BasePluginDescriptor::CreateVacModel         ( DefaultPluginParamValModelPtr&, ITimeEvaluatorPtr ) const
{
    return std::make_shared< DefaultParamValModel >();
}

// *********************************
//
template< typename T >
void                                     BasePluginDescriptor::AddParam            ( DefaultParamValModelPtr&, ITimeEvaluatorPtr timeEvaluator, std::string name, const T& defaultValue, bool addValue, bool isState  ) const
{
    assert( false );
}

// *********************************
//
template<>
void                                     BasePluginDescriptor::AddParam< float >   ( DefaultParamValModelPtr& model, ITimeEvaluatorPtr timeEvaluator, std::string name, const float& defaultValue, bool addValue, bool isState ) const
{
    auto param = ParametersFactory::CreateParameterFloat( name, timeEvaluator );
    model->AddParameter( param );
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
        auto updater = std::make_shared< SimpleFloatStateUpdater >( state, model->GetValue( name ) );
        model->AddState( name, state, updater );
    }
}

} //model
} //bv
