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

	if ( uid != DefaultRectPluginDesc::UID() && uid != DefaultTextPluginDesc::UID() )
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
std::string             DefaultGradientPluginDesc::VertexShaderSource        ()
{
    return "../dep/media/shaders/combinations/lg.vert";
}

// *******************************
//
std::string             DefaultGradientPluginDesc::PixelShaderSource         ()
{
    return "../dep/media/shaders/combinations/lg.frag";
}

// *******************************
//
std::string             DefaultGradientPluginDesc::TextureName               ()
{
    return "Grad0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultGradientPlugin::DefaultGradientPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
	, m_pscInitialized(false)
{
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultGradientPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultGradientPluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );

    InitAttributesChannel( prev );
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
	if(!m_pscInitialized) // FIXME this sucks so very much I can't breathe!!!
	{
		auto prevTexturesData = m_prevPlugin->GetPixelShaderChannel()->GetTexturesData();

		for(auto t : prevTexturesData->GetTextures())
			m_psc->GetTexturesDataImpl()->AddTexture(static_cast<DefaultTextureDescriptor*>(t));

		const_cast<DefaultGradientPlugin*>(this)->m_pscInitialized = true;
	}
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
                    if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, m_texCoordChannelIndex ) )
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

    if( m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() ) //FIXME: additionalna hackierka
    {
        if( m_vaChannel != nullptr )
        {
            m_vaChannel->ClearAll();
        }

		InitAttributesChannel( m_prevPlugin );	
	}

    //auto wX = GetWrapModeX();
    //auto wY = GetWrapModeY();
    //auto fm = GetFilteringMode();

    //if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() || StateChanged( wX, wY, fm, attachmentMode ) )
    //{
    //    UpdateState( wX, wY, fm, attachmentMode );
        m_vaChannel->SetNeedsAttributesUpdate( true );
    //}
    //else
    //{
        //m_vaChannel->SetNeedsAttributesUpdate( false );
    //}

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void DefaultGradientPlugin::InitAttributesChannel( IPluginPtr prev )
{
	auto prevGeomChannel = prev->GetVertexAttributesChannel();
    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    if( prevGeomChannel == nullptr ) //FIXME: hackierka
    {
        assert( prev->GetTypeUid() == DefaultTextPluginDesc::UID() );

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

            m_texCoordChannelIndex = vaChannelDesc.GetNumVertexChannels();

            //Only one texture
            vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            auto vaChannel = VertexAttributesChannelPtr( new VertexAttributesChannel( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() ) );
            m_vaChannel = vaChannel;
        }

		//m_vaChannel->ClearConnectedComponent();

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

//namespace {
//
//// *************************************
//// FIXME: implement int parameters and bool parameters
//template< typename EnumClassType >
//inline EnumClassType EvaluateAsInt( ParamFloat * param )
//{
//    int val = int( param->Evaluate() );
//
//    return EnumClassType( val );
//}
//
//// *************************************
//// FIXME: implement int parameters and bool parameters
//template< typename EnumClassType >
//inline EnumClassType EvaluateAsInt( ParamFloatPtr param )
//{
//    int val = int( param->Evaluate() );
//
//    return EnumClassType( val );
//}
//
//} //anonymous

// *************************************
// 
void                                        DefaultGradientPlugin::UpdateState                 ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am )
{
}

} // model
} // bv
