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
IPluginPtr              DefaultTransformPluginDesc::CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTransformPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTransformPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr model         = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr trModel             = std::make_shared< DefaultParamValModel >();
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
DefaultTransformPlugin::DefaultTransformPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_transformChannel( nullptr )
    , m_paramValModel( model )
{ 
    auto trModel = model->TransformChannelModelImpl();

    assert( trModel );

    ValueMat4PtrVec typedVals;

    for( auto val : trModel->GetValuesNC() )
    {
        typedVals.push_back( QueryTypedValue< ValueMat4Ptr >( val ) );
    }

    m_transformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create( prev, typedVals, false ) ); //<3
}

// *************************************
//
DefaultTransformPlugin::~DefaultTransformPlugin ()
{
}

// *************************************
//
ITransformChannelConstPtr           DefaultTransformPlugin::GetTransformChannel         () const
{
    return m_transformChannel;
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
