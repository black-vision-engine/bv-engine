#include "stdafx.h"

#include "DefaultVideoStreamDecoderPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultVideoStreamDescriptor.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Assets/DefaultAssets.h"


namespace bv { namespace model {


const std::string        DefaultVideoStreamDecoderPlugin::PARAM::ALPHA          = "alpha";
const std::string        DefaultVideoStreamDecoderPlugin::PARAM::TX_MAT         = "txMat";
const std::string        DefaultVideoStreamDecoderPlugin::PARAM::DECODER_STATE  = "state";
const std::string        DefaultVideoStreamDecoderPlugin::PARAM::SEEK_OFFSET    = "offset";

typedef ParamEnum< DefaultVideoStreamDecoderPlugin::DecoderMode > ParamEnumDM;


// ***********************
//
template<>
static IParameterPtr    ParametersFactory::CreateTypedParameter< DefaultVideoStreamDecoderPlugin::DecoderMode > ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultVideoStreamDecoderPlugin::DecoderMode >( name, timeline );
}

// ***********************
//
VoidPtr    ParamEnumDM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"


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
    ModelHelper helper( timeEvaluator );
    auto model  = helper.GetModel();

    helper.CreatePluginModel();
    helper.AddSimpleParam( DefaultVideoStreamDecoderPlugin::PARAM::SEEK_OFFSET, glm::vec2( 0.f ), true );
    helper.AddParam< IntInterpolator, DefaultVideoStreamDecoderPlugin::DecoderMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumDM >
        ( DefaultVideoStreamDecoderPlugin::PARAM::DECODER_STATE, DefaultVideoStreamDecoderPlugin::DecoderMode::STOP, true, true );

    helper.CreateVSModel();
    helper.AddTransformParam( DefaultVideoStreamDecoderPlugin::PARAM::TX_MAT, true );

    helper.CreatePSModel();
    helper.AddSimpleParam( DefaultVideoStreamDecoderPlugin::PARAM::ALPHA, 1.f, true );

    return model;
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
    BasePlugin::SetPrevPlugin( prev );

	InitVertexAttributesChannel();

	HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    ctx->alphaCtx->blendEnabled = true;
    ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
    ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// *************************************
// 
DefaultVideoStreamDecoderPlugin::DefaultVideoStreamDecoderPlugin					( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
	: BasePlugin< IPlugin >( name, uid, prev, model )
	, m_psc( nullptr )
	, m_vsc( nullptr )
	, m_vaChannel( nullptr )
	, m_decoder( nullptr )
    , m_prevOffsetCounter( 0 )
    , m_prevDecoderModeTime( 0 )
    , m_prevOffsetTime( 0 )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel(), nullptr );
	m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

	SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< VideoStreamAssetDesc >() );

    m_decoderModeParam = QueryTypedParam< std::shared_ptr< ParamEnum< DecoderMode > > >( GetParameter( PARAM::DECODER_STATE ) );
    m_decoderModeParam->SetGlobalCurveType( CurveType::CT_POINT );
    m_prevDecoderMode = m_decoderModeParam->Evaluate();
    
    m_offsetParam = QueryTypedParam< ParamVec2Ptr >( GetParameter( PARAM::SEEK_OFFSET ) );
    m_offsetParam->SetGlobalCurveType( CurveType::CT_POINT );
}

// *************************************
// 
DefaultVideoStreamDecoderPlugin::~DefaultVideoStreamDecoderPlugin					()
{
}

// *************************************
// 
bool							DefaultVideoStreamDecoderPlugin::IsValid     () const
{
	return ( m_vaChannel && m_prevPlugin->IsValid() );
}

// *************************************
// 
bool                            DefaultVideoStreamDecoderPlugin::LoadResource		( AssetDescConstPtr assetDescr )
{
	auto vstreamAssetDescr = QueryTypedDesc< VideoStreamAssetDescConstPtr >( assetDescr );

    if ( vstreamAssetDescr != nullptr )
    {
		m_decoder = std::make_shared< FFmpegVideoDecoder >( vstreamAssetDescr );

		auto vsDesc = std::make_shared< DefaultVideoStreamDescriptor >( DefaultVideoStreamDecoderPluginDesc::TextureName(), MemoryChunk::Create( m_decoder->GetFrameSize() ), m_decoder->GetWidth(), m_decoder->GetHeight(), vstreamAssetDescr->GetTextureFormat(), DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE );
        if( vsDesc != nullptr )
		{
			vsDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );

			auto txData = m_psc->GetTexturesDataImpl();
			txData->SetTexture( 0, vsDesc );

			SetAsset( 0, LAsset( vsDesc->GetName(), assetDescr, vsDesc->GetSamplerState() ) );

			HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

			return true;
		}
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
IPixelShaderChannelPtr		         DefaultVideoStreamDecoderPlugin::GetPixelShaderChannel       () const
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
   	BasePlugin::Update( t );
    MarkOffsetChanges();

    HandleDecoder();

	HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
	if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) )
	{
		InitVertexAttributesChannel();
	}
	HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );
	
	//update texture with video data
	auto mediaData = m_decoder->GetVideoMediaData();
	if( mediaData.frameData != nullptr )
	{
		std::static_pointer_cast< DefaultVideoStreamDescriptor >( m_psc->GetTexturesDataImpl()->GetTexture( 0 ) )->SetBits( mediaData.frameData );
	}

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void									DefaultVideoStreamDecoderPlugin::InitVertexAttributesChannel		()
{
	if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
	{
		m_vaChannel = nullptr;
		return;
	}

	auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();
	auto prevCC = prevGeomChannel->GetComponents();

    //Only one texture
	VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );
	if( !vaChannelDesc.GetAttrChannelDescriptor( AttributeSemantic::AS_TEXCOORD ) )
	{
		vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
	}
	
	if( !m_vaChannel )
	{		
		m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
	}
	else
	{
		m_vaChannel->ClearAll();
		m_vaChannel->SetDescriptor( vaChannelDesc );
	}

	auto desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    for( unsigned int i = 0; i < prevCC.size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();

        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevCC[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

		auto posChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
		if( posChannel && !prevConnComp->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) )
		{
			//FIXME: only one texture - convex hull calculations
			auto uvs = new model::Float2AttributeChannel( desc, DefaultVideoStreamDecoderPluginDesc::TextureName(), true );
			auto uvsPtr = Float2AttributeChannelPtr( uvs );
			
			Helper::UVGenerator::generateUV( reinterpret_cast< const glm::vec3 * >( posChannel->GetData() ), posChannel->GetNumEntries(),
											uvsPtr, glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, -1.0, 0.0 ), true );

			connComp->AddAttributeChannel( uvsPtr );
		}

        m_vaChannel->AddConnectedComponent( connComp );
    }
	
    assert( prevGeomChannel->GetComponents().size() > 0 );
}

// *************************************
//
void                                DefaultVideoStreamDecoderPlugin::HandleDecoder  ()
{
    if( m_decoder )
    {
        //order matters - update offset first then mode
        auto offset = m_offsetParam->Evaluate();
        auto mode =  m_decoderModeParam->Evaluate();

        auto decoderModeTime = m_decoderModeParam->GetLocalEvaluationTime();
        auto offsetTime = m_offsetParam->GetLocalEvaluationTime();

        if( ( m_prevOffsetCounter != offset[ 1 ] ) || ( m_prevOffsetTime > offsetTime ) )
        {
            //edge case - eof
            if( mode == DecoderMode::PLAY )
            {
                m_decoder->Start();
            }

            m_decoder->Seek( offset[ 0 ] );
            m_prevOffsetCounter = offset[ 1 ];
        }

        if( m_prevDecoderMode != mode )
        {
            switch( mode )
            {
            case DecoderMode::PLAY:
                m_decoder->Start(); break;
            case DecoderMode::STOP:
                m_decoder->Stop(); break;
            case DecoderMode::PAUSE:
                m_decoder->Pause(); break;
            }

            m_prevDecoderMode = mode;
        }

        //edge case - loop
        if( ( m_prevDecoderModeTime > decoderModeTime ) && ( mode == DecoderMode::PLAY ) )
        {
            m_decoder->Start();
        }

        m_prevDecoderModeTime = decoderModeTime;
    }
}

// *************************************
//
void                                DefaultVideoStreamDecoderPlugin::MarkOffsetChanges  ()
{
    auto counter = 0;
    const auto keys = m_offsetParam->AccessInterpolator().GetKeys();
    for( auto & key : keys )
    {
        m_offsetParam->SetVal( glm::vec2( key.val[ 0 ], ++counter ), key.t );
    }
}

} //model
} //bv