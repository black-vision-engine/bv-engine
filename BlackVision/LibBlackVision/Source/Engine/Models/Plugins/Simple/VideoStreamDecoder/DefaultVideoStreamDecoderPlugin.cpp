#include "DefaultVideoStreamDecoderPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultVideoStreamDescriptor.h"

#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultVideoStreamDecoderPluginDesc::DefaultVideoStreamDecoderPluginDesc			()
    : BasePluginDescriptor( UID(), "video_stream_decoder", "tx" )
{
}

// *******************************
//
IPluginPtr              DefaultVideoStreamDecoderPluginDesc::CreatePlugin           ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultVideoStreamDecoderPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultVideoStreamDecoderPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleVec4EvaluatorPtr      borderColorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator		= ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator		= ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
	SimpleFloatEvaluatorPtr		wrapModeXEvaluator	= ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "wrapModeX", timeEvaluator );
	SimpleFloatEvaluatorPtr		wrapModeYEvaluator	= ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "wrapModeY", timeEvaluator );

    ParamFloatPtr  paramFilteringMode = ParametersFactory::CreateParameterFloat( "filteringMode", timeEvaluator );
    ParamFloatPtr  paramAttachMode    = ParametersFactory::CreateParameterFloat( "attachmentMode", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );

    psModel->AddParameter( paramFilteringMode );
    psModel->AddParameter( paramAttachMode );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
	wrapModeXEvaluator->Parameter()->SetVal( static_cast<int>( TextureWrappingMode::TWM_REPEAT ), TimeType( 0.0 ) ); 
	wrapModeYEvaluator->Parameter()->SetVal( static_cast<int>( TextureWrappingMode::TWM_REPEAT ), TimeType( 0.0 ) ); 

    paramFilteringMode->SetVal( (float) TextureFilteringMode::TFM_LINEAR, TimeType( 0.f ) );
    paramAttachMode->SetVal( (float) TextureAttachmentMode::MM_ATTACHED, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultVideoStreamDecoderPluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return false;
    }

    auto  vac = plugin->GetVertexAttributesChannel();
    if ( vac == nullptr )
    {
        return false;
    }

    //auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
    //if ( numChannels != 1 ) //only vertex attribute data allowed here
    //{
    //    return false;
    //}

    return true;
}

// *******************************
//
std::string             DefaultVideoStreamDecoderPluginDesc::UID                       ()
{
    return "DEFAULT_VIDEO_STREAM_DECODER";
}

// *******************************
//
std::string             DefaultVideoStreamDecoderPluginDesc::TextureName               ()
{
    return "Tex0";
}


// ************************************************************************* PLUGIN *************************************************************************

void					DefaultVideoStreamDecoderPlugin::SetPrevPlugin               ( IPluginPtr prev )
{
	__super::SetPrevPlugin( prev );

	if( prev == nullptr )
		return;

	InitAttributesChannel( prev );
}

// *************************************
// 
DefaultVideoStreamDecoderPlugin::DefaultVideoStreamDecoderPlugin					( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
	: BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
	, m_psc( nullptr )
	, m_vsc( nullptr )
	, m_vaChannel( nullptr )
	, m_paramValModel( model )
	, m_decoderThread( nullptr )
	, m_frameId( 0 )
	, m_started( false )
{
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel(), nullptr ) );
	m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() ) );

	SetPrevPlugin( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    
    ctx->alphaCtx->blendEnabled = true;
    ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
    ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;

	m_texturesData = m_psc->GetTexturesDataImpl();

	//Direct param state access (to bypass model querying)
	auto psModel = PixelShaderChannelModel();
}

// *************************************
// 
DefaultVideoStreamDecoderPlugin::~DefaultVideoStreamDecoderPlugin					()
{
	if( m_decoderThread )
	{
		m_decoderThread->Join();
		delete m_decoderThread;
	}
}

// *************************************
// 
bool                            DefaultVideoStreamDecoderPlugin::LoadResource		( AssetDescConstPtr assetDescr )
{
	auto vstreamAssetDescr = QueryTypedDesc< VideoStreamAssetDescConstPtr >( assetDescr );

    if ( vstreamAssetDescr != nullptr )
    {
		m_decoderThread = new VideoStreamDecoderThread( vstreamAssetDescr->GetStreamPath() );

		auto data = m_decoderThread->GetFrameData( m_frameId );

		auto vsDesc = new DefaultVideoStreamDescriptor( DefaultVideoStreamDecoderPluginDesc::TextureName(), data, m_decoderThread->GetWidth(), m_decoderThread->GetHeight(), TextureFormat::F_A8R8G8B8, DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE );

		auto txData = m_psc->GetTexturesDataImpl();
        if( vsDesc != nullptr )
		{
            if( txData->GetTextures().size() == 0 )
			{
				txData->AddTexture( vsDesc );
			}
			else
			{
				txData->SetTexture( 0, vsDesc );
			}
		}

        return true;
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultVideoStreamDecoderPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultVideoStreamDecoderPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultVideoStreamDecoderPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultVideoStreamDecoderPlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused variable

	if( !m_started )
	{
		m_decoderThread->Start();
		m_started = true;
	}

    m_paramValModel->Update();

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

    if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
    {
        m_vaChannel->SetNeedsAttributesUpdate( true );
    }
    else
    {
        m_vaChannel->SetNeedsAttributesUpdate( false );
    }
	
	auto txData = m_psc->GetTexturesDataImpl();
	auto vsDesc = static_cast< DefaultVideoStreamDescriptor * >( txData->GetTexture( 0 ) );

	if( m_decoderThread->NewVideoFrameArrived( m_frameId ) )
	{
		vsDesc->SetBits( m_decoderThread->GetFrameData( m_frameId ) );
	}

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void									DefaultVideoStreamDecoderPlugin::InitAttributesChannel		( IPluginPtr prev )
{
    auto prevGeomChannel = prev->GetVertexAttributesChannel();
    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

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

		//FIXME: only one texture - convex hull calculations
        float minX = 100000.0f, minY = 100000.0f;
        float maxX = 0.0f, maxY = 0.0f;

        //convex hull - make sure that prevCompChannels[ 0 ] is indeed a positional channel
        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );

            minX = std::min( minX, pos[ j ].x );
            minY = std::min( minY, pos[ j ].y );
            maxX = std::max( maxX, pos[ j ].x );
            maxY = std::max( maxY, pos[ j ].y );
        }

        auto verTexAttrChannel = new model::Float2AttributeChannel( desc, DefaultVideoStreamDecoderPluginDesc::TextureName(), true );

        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );
            verTexAttrChannel->AddAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
        }

        connComp->AddAttributeChannel( AttributeChannelPtr( verTexAttrChannel ) );

        m_vaChannel->AddConnectedComponent( connComp );
    }
}

} }