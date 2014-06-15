#include "DefaultTransformPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTransformPluginDesc::DefaultTransformPluginDesc                          ()
    : BasePluginDescriptor( UID(), "transform" )    
{
}

// *******************************
//
IPlugin *               DefaultTransformPluginDesc::CreatePlugin                ( const std::string & name, const IPlugin * prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTransformPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModel *    DefaultTransformPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModel * model          = new DefaultPluginParamValModel();
    DefaultParamValModel * trModel              = new DefaultParamValModel();
    auto evaluator                              = ParamValEvaluatorFactory::CreateTransformVecEvaluator( "simple_transform", timeEvaluator );

    trModel->RegisterAll( evaluator );
    model->SetTransformChannelModel( trModel );

    //Set default values
    evaluator->Parameter()->Transform( 0 ).InitializeDefaultSRT();

    return model;
}

// *******************************
//
std::string             DefaultTransformPluginDesc::UID                         ()
{
    return "DEFAULT_TRANSFORM";
}


// ************************************************************************* PLUGIN ************************************************************************* 

// *******************************
//
DefaultTransformPlugin::DefaultTransformPlugin  ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_transformChannel( nullptr )
    , m_paramValModel( model )
{ 
    auto trModel = model->TransformChannelModelImpl();

    assert( trModel );

    ValueMat4PtrVec typedVals;

    for( auto val : trModel->GetValuesNC() )
    {
        typedVals.push_back( QueryTypedValue< ValueMat4 >( val ) );
    }

    m_transformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create( prev, typedVals, false ) );
}

// *************************************
//
DefaultTransformPlugin::~DefaultTransformPlugin ()
{
}

// *************************************
//
const ITransformChannel *           DefaultTransformPlugin::GetTransformChannel         () const
{
    return m_transformChannel.get();
}

// *************************************
//
void                                DefaultTransformPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update();
    m_transformChannel->PostUpdate();
}

} // model
} // bv
