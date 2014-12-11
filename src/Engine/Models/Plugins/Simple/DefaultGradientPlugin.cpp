#include "DefaultGradientPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Resources/IPluginResourceDescr.h"

#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPrismPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAnimationPlugin.h"

namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGradientPluginDesc::DefaultGradientPluginDesc                          ()
    : BasePluginDescriptor( UID(), "linear_gradient" )
{
}

// *******************************
//
IPluginPtr              DefaultGradientPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultGradientPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultGradientPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

	SimpleVec4EvaluatorPtr		color1Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator("color1", timeEvaluator );
	SimpleVec4EvaluatorPtr		color2Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator("color2", timeEvaluator );

	SimpleVec2EvaluatorPtr		point1Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator("point1", timeEvaluator );
	SimpleVec2EvaluatorPtr		point2Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator("point2", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
	psModel->RegisterAll( color1Evaluator );
	psModel->RegisterAll( color2Evaluator );
	psModel->RegisterAll( point1Evaluator );
	psModel->RegisterAll( point2Evaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
	color1Evaluator->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );
	color2Evaluator->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );
	point1Evaluator->Parameter()->SetVal( glm::vec2( 0.0f, 1.0f ), TimeType( 0.0 ) );
	point2Evaluator->Parameter()->SetVal( glm::vec2( 1.0f, 0.0f ), TimeType( 0.0 ) );

    return model;
}

// *******************************
//
bool                   DefaultGradientPluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return false;
    }

    //auto  vac = plugin->GetVertexAttributesChannel();
    //if ( vac == nullptr )
    //{
    //    return false;
    //}

    //auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
    //if ( numChannels != 1 ) //only vertex attribute data allowed here
    //{
    //    return false;
    //}

    auto uid = plugin->GetTypeUid();

	if ( uid != DefaultRectPluginDesc::UID() && uid != DefaultTextPluginDesc::UID() && uid != DefaultTransformPluginDesc::UID() && uid != DefaultTimerPluginDesc::UID() && uid != DefaultPrismPluginDesc::UID() )
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string             DefaultGradientPluginDesc::UID                       ()
{
    return "DEFAULT_LINEAR_GRADIENT";
}

// *******************************
//
//std::string             DefaultGradientPluginDesc::VertexShaderSource        ()
//{
//    return "../dep/media/shaders/combinations/lg.vert";
//}
//
//// *******************************
////
//std::string             DefaultGradientPluginDesc::PixelShaderSource         ()
//{
//    return "../dep/media/shaders/combinations/lg.frag";
//}
//
// *******************************
//
std::string             DefaultGradientPluginDesc::TextureName               ()
{
    return "Grad0";
}

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultGradientPlugin::DefaultGradientPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
{
    //m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultGradientPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    //m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultGradientPluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );
	m_psc = DefaultPixelShaderChannelPtr( new DefaultPixelShaderChannel( "", model->GetPixelShaderChannelModel() ) );
	m_vsc = DefaultVertexShaderChannelPtr( new DefaultVertexShaderChannel( "", model->GetVertexShaderChannelModel() ) );

    InitAttributesChannel( prev );

    if( prev->GetTypeUid() == DefaultTexturePluginDesc::UID() || prev->GetTypeUid() == DefaultAnimationPluginDesc::UID() || prev->GetTypeUid() == DefaultTextPluginDesc::UID() || prev->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        //FIXME: set textures data from prev plugin to this plugin
        auto prev_psc = std::const_pointer_cast< ITexturesData >( prev->GetPixelShaderChannel()->GetTexturesData() );
        //FIXME: this line causes changes to Texture Plugin data via current pointer - quite shitty
        m_psc->OverrideTexturesData( std::static_pointer_cast< DefaultTexturesData >( prev_psc ) );
    }
}

// *************************************
// 
DefaultGradientPlugin::~DefaultGradientPlugin         ()
{
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultGradientPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultGradientPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultGradientPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultGradientPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update();

    if( m_prevPlugin->GetVertexAttributesChannel() && m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() ) //FIXME: additionalna hackierka
    {
        if( m_vaChannel != nullptr )
        {
            m_vaChannel->ClearAll();
        }

		InitAttributesChannel( m_prevPlugin );	
		m_vaChannel->SetNeedsTopologyUpdate( true );
	}

        //m_vaChannel->SetNeedsAttributesUpdate( true );

    //m_vsc->PostUpdate();
    //m_psc->PostUpdate();    
}

// *************************************
//
void DefaultGradientPlugin::InitAttributesChannel( IPluginPtr prev )
{
	auto prevGeomChannel = prev->GetVertexAttributesChannel();
    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR ); // TOCHECK is that right?

    if( prevGeomChannel == nullptr ) //FIXME: hackierka
    {
		assert( prev->GetTypeUid() == DefaultTextPluginDesc::UID() || prev->GetTypeUid() == DefaultTransformPluginDesc::UID() || prev->GetTypeUid() == DefaultTimerPluginDesc::UID() || prev->GetTypeUid() == DefaultPrismPluginDesc::UID() );

        return;
    }


    //FIXME: only one texture - convex hull calculations
    float minX = 100000.0f, minY = 100000.0f;
    float maxX = 0.0f, maxY = 0.0f;


    for( unsigned int i = 0; i < prevGeomChannel->GetComponents().size(); ++i )
    {
        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevGeomChannel->GetComponents()[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        //convex hull - make sure that prevCompChannels[ 0 ] is indeed a positional channel
        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );

            minX = std::min( minX, pos[ j ].x );
            minY = std::min( minY, pos[ j ].y );
            maxX = std::max( maxX, pos[ j ].x );
            maxY = std::max( maxY, pos[ j ].y );
        }
	}
	
    for( unsigned int i = 0; i < prevGeomChannel->GetComponents().size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();
        VertexAttributesChannelDescriptor vaChannelDesc;

        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevGeomChannel->GetComponents()[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

        if( m_vaChannel == nullptr )
        {
            for( auto prevCompCh : prevCompChannels )
            {
                auto prevCompChDesc = prevCompCh->GetDescriptor();
                vaChannelDesc.AddAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
            }

            //Only one texture
            vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR ); // TOCHECK is it needed?

            auto vaChannel = VertexAttributesChannelPtr( new VertexAttributesChannel( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() ) );
            m_vaChannel = vaChannel;
        }

        auto verTexAttrChannel = new model::Float2AttributeChannel( desc, DefaultGradientPluginDesc::TextureName(), true );

        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );
            verTexAttrChannel->AddAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
        }

        connComp->AddAttributeChannel( AttributeChannelPtr( verTexAttrChannel ) );

        m_vaChannel->AddConnectedComponent( connComp );
    }
}

} // model
} // bv
