#include "DefaultTransformPlugin.h"

#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {

// *************************************
//
DefaultTransformPlugin::DefaultTransformPlugin  ( const IPlugin * prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( "dupa", "dupa", prev,  std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_transformChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_transformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create( prev, TransformChannelModel(), false ) );
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
    m_paramValModel->Update( t );
    m_transformChannel->PostUpdate();
}


// ********************************************************* DESCRIPTOR *********************************************************

// *************************************
//
DefaultPluginParamValModel * DefaultTransformPluginDesc::CreateModel ( bool setDefaultValues )
{
    DefaultPluginParamValModel * model          = new DefaultPluginParamValModel();
    DefaultParamValModel * trModel              = new DefaultParamValModel();
    TransformVecParamValEvaluator * evaluator   = ParamValEvaluatorFactory::CreateTransformVecEvaluator( "simple_transform" );

    trModel->RegisterAll( evaluator );
    model->SetTransformChannelModel( trModel );

    if ( setDefaultValues )
    {
        evaluator->Param()->Transform( 0 ).InitializeDefaultSRT();
    }

    return model;
}

// *************************************
//
DefaultTransformPlugin *            DefaultTransformPluginDesc::CreatePlugin            ( const IPlugin * prev, bool setDefaultValues )
{
    return new DefaultTransformPlugin( prev, DefaultPluginParamValModelPtr( DefaultTransformPluginDesc::CreateModel( setDefaultValues ) ) );
}

} // model
} // bv
