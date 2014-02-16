#include "DefaultTransformPlugin.h"

#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"


namespace bv { namespace model {

// *************************************
//
DefaultTransformPlugin::DefaultTransformPlugin  ( const IPlugin * prev, IPluginParamValModel * model )
    : BasePlugin( prev )
    , m_transformChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_transformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create( prev, m_paramValModel->GetTransformModel(), false ) );
}

// *************************************
//
DefaultTransformPlugin::~DefaultTransformPlugin ()
{
    delete m_paramValModel;
}

// *************************************
//
const IDefaultTransformChannel *    DefaultTransformPlugin::GetDefaultTransformChannel  () const
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

// *************************************
//
DefaultTransformPlugin *            DefaultTransformPlugin::Create                      ( const IPlugin * prev )
{
    //FIXME: create model
    //IPluginParamValModel * model = CreateModel();
    IPluginParamValModel * model = nullptr;

    return new DefaultTransformPlugin( prev, model );
}

} // model
} // bv
