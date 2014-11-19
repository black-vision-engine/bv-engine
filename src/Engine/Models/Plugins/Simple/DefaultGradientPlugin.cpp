#include "DefaultGradientPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGradientPluginDesc::DefaultGradientPluginDesc                          ()
    : BasePluginDescriptor( UID(), "gradient" )
{
}

// *******************************
//
IPluginPtr              DefaultGradientPluginDesc::CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultGradientPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultGradientPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    SimpleVec4EvaluatorPtr evaluator1     = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color1", timeEvaluator );
    SimpleVec4EvaluatorPtr evaluator2     = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color2", timeEvaluator );

    psModel->RegisterAll( evaluator1 );
    psModel->RegisterAll( evaluator2 );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values
    evaluator1->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );
    evaluator2->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );

    return model;
}

// *******************************
//
std::string             DefaultGradientPluginDesc::UID                         ()
{
    return "DEFAULT_LINEAR_GRADIENT";
}

// *******************************
//
std::string             DefaultGradientPluginDesc::PixelShaderSource           ()
{
    return "../dep/media/shaders/combinations/lingrad.frag";
}


// ************************************************************************* PLUGIN ************************************************************************* 

// *******************************
//
DefaultGradientPlugin::DefaultGradientPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_paramValModel( model )
{ 
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultGradientPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_psc->GetRendererContext()->alphaCtx->blendEnabled = true;
}

// *************************************
//
DefaultGradientPlugin::~DefaultGradientPlugin ()
{
}

// *************************************
//
IPixelShaderChannelConstPtr         DefaultGradientPlugin::GetPixelShaderChannel       () const
{
    return m_psc;    
}

// *************************************
//
void                                DefaultGradientPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update();

    //auto attachmentMode = GetAttachementMode();

    //if( attachmentMode == TextureAttachmentMode::MM_FREE )
    {
        if( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
        {
            for( unsigned int i = 0; i < m_vaChannel->GetComponents().size(); ++i )
            {
                auto connComp = m_vaChannel->GetConnectedComponent( i );
                auto compChannels = connComp->GetAttributeChannels();

                if( auto posChannel = AttributeChannel::GetPositionChannel( compChannels ) )
                {
                    if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, /*m_texCoordChannelIndex*/0 ) )
                    {
                        auto & verts  = std::dynamic_pointer_cast< Float3AttributeChannel >( posChannel )->GetVertices();
                        auto & uvs    = std::dynamic_pointer_cast< Float2AttributeChannel >( uvChannel )->GetVertices();

                        for( unsigned int i = 0; i < verts.size(); ++i )
                        {
                            uvs[ i ].x = verts[ i ].x;
                            uvs[ i ].y = verts[ i ].y;
                        }
                    }
                }
            }
        }
    }

    //auto wX = GetWrapModeX();
    //auto wY = GetWrapModeY();
    //auto fm = GetFilteringMode();

    //if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() /*|| StateChanged( wX, wY, fm, attachmentMode )*/ )
    //{
    //    UpdateState( wX, wY, fm, attachmentMode );
    //    m_vaChannel->SetNeedsAttributesUpdate( true );
    //}
    //else
    {
        m_vaChannel->SetNeedsAttributesUpdate( false );
    }

    //m_vsc->PostUpdate();
    m_psc->PostUpdate();    
	
	//m_paramValModel->Update();
 //   m_pixelShaderChannel->PostUpdate();
}

} // model
} // bv
