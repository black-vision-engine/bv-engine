#include "stdafx.h"

#include "DefaultAVDecoderPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultVideoStreamDescriptor.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Events/Events.h"


namespace bv { namespace model {


const std::string        DefaultAVDecoderPlugin::PARAM::ALPHA          = "alpha";
const std::string        DefaultAVDecoderPlugin::PARAM::TX_MAT         = "txMat";
const std::string        DefaultAVDecoderPlugin::PARAM::DECODER_STATE  = "state";
const std::string        DefaultAVDecoderPlugin::PARAM::SEEK_OFFSET    = "offset";
const std::string        DefaultAVDecoderPlugin::PARAM::LOOP_ENABLED   = "loopEnabled";
const std::string        DefaultAVDecoderPlugin::PARAM::LOOP_COUNT     = "loopCount";

typedef ParamEnum< DefaultAVDecoderPlugin::DecoderMode > ParamEnumDM;


// ***********************
//
template<>
static IParameterPtr    ParametersFactory::CreateTypedParameter< DefaultAVDecoderPlugin::DecoderMode > ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultAVDecoderPlugin::DecoderMode >( name, timeline );
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
DefaultAVDecoderPluginDesc::DefaultAVDecoderPluginDesc			()
    : BasePluginDescriptor( UID(), "video_decoder", "tx" )
{
}

// *******************************
//
IPluginPtr                      DefaultAVDecoderPluginDesc::CreatePlugin           ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultAVDecoderPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultAVDecoderPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );
    auto model  = helper.GetModel();

    helper.CreatePluginModel();
    helper.AddSimpleParam( DefaultAVDecoderPlugin::PARAM::SEEK_OFFSET, glm::vec2( 0.f ), true );
    helper.AddParam< IntInterpolator, DefaultAVDecoderPlugin::DecoderMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumDM >
        ( DefaultAVDecoderPlugin::PARAM::DECODER_STATE, DefaultAVDecoderPlugin::DecoderMode::STOP, true, true );
    helper.AddSimpleParam( DefaultAVDecoderPlugin::PARAM::LOOP_ENABLED, false, false );
    helper.AddSimpleParam( DefaultAVDecoderPlugin::PARAM::LOOP_COUNT, 0, true, true );

    helper.CreateVSModel();
    helper.AddTransformParam( DefaultAVDecoderPlugin::PARAM::TX_MAT, true );
    auto param = helper.GetModel()->GetVertexShaderChannelModel()->GetParameter( DefaultAVDecoderPlugin::PARAM::TX_MAT );
    SetParameterCenterMass( param, 0.0f, glm::vec3( 0.5, 0.5, 0.0 ) );

    helper.CreatePSModel();
    helper.AddSimpleParam( DefaultAVDecoderPlugin::PARAM::ALPHA, 1.f, true );

    return model;
}

// *******************************
//
std::string                     DefaultAVDecoderPluginDesc::UID                       ()
{
    return "DEFAULT_VIDEO_DECODER";
}

// *******************************
//
std::string                     DefaultAVDecoderPluginDesc::TextureName               ()
{
    return "Tex0";
}


// ************************************************************************* PLUGIN *************************************************************************

void					DefaultAVDecoderPlugin::SetPrevPlugin               ( IPluginPtr prev )
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
DefaultAVDecoderPlugin::DefaultAVDecoderPlugin					( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
	: BasePlugin< IPlugin >( name, uid, prev, model )
	, m_psc( nullptr )
	, m_vsc( nullptr )
	, m_vaChannel( nullptr )
	, m_decoder( nullptr )
    , m_prevOffsetCounter( 1 )
    , m_prevDecoderModeTime( 0 )
    , m_prevOffsetTime( 0 )
    , m_isFinished( false )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel(), nullptr );
	m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    m_audioChannel = DefaultAudioChannel::Create( 44100, AudioFormat::STEREO16 );

	SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< AVAssetDesc >() );

    m_decoderModeParam = QueryTypedParam< std::shared_ptr< ParamEnum< DecoderMode > > >( GetParameter( PARAM::DECODER_STATE ) );
    m_decoderModeParam->SetGlobalCurveType( CurveType::CT_POINT );
    
    m_offsetParam = QueryTypedParam< ParamVec2Ptr >( GetParameter( PARAM::SEEK_OFFSET ) );
    m_offsetParam->SetGlobalCurveType( CurveType::CT_POINT );

    m_loopEnabledParam = QueryTypedParam< ParamBoolPtr >( GetParameter( PARAM::LOOP_ENABLED ) );
    m_loopCountParam = QueryTypedParam< ParamIntPtr >( GetParameter( PARAM::LOOP_COUNT ) );

    m_decoderMode =  m_decoderModeParam->Evaluate();
}

// *************************************
// 
DefaultAVDecoderPlugin::~DefaultAVDecoderPlugin					()
{
}

// *************************************
// 
bool							DefaultAVDecoderPlugin::IsValid     () const
{
	return ( m_vaChannel && m_prevPlugin->IsValid() );
}

// *************************************
// 
bool                            DefaultAVDecoderPlugin::LoadResource		( AssetDescConstPtr assetDescr )
{
	m_assetDesc = QueryTypedDesc< AVAssetDescConstPtr >( assetDescr );

    if ( m_assetDesc )
    {
        auto asset = LoadTypedAsset<AVAsset>( assetDescr );
        if( asset )
        {
		    m_decoder = std::make_shared< FFmpegAVDecoder >( asset );

            if( m_decoder->HasVideo() )
            {
                if( m_decoder->HasAudio() )
                {
                    m_audioChannel->SetFrequency( m_decoder->GetSampleRate() );
                    m_audioChannel->SetFormat( m_decoder->GetAudioFormat() );
                }

                //get the first frame
                while( !m_decoder->NextVideoDataReady( 0 ) );

		        auto vsDesc = std::make_shared< DefaultVideoStreamDescriptor >( DefaultAVDecoderPluginDesc::TextureName(),
                    MemoryChunk::Create( m_decoder->GetVideoFrameSize() ), m_decoder->GetWidth(), m_decoder->GetHeight(), 
                    m_assetDesc->GetTextureFormat(), DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE );

                if( vsDesc )
		        {
			        vsDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );

			        auto txData = m_psc->GetTexturesDataImpl();
			        txData->SetTexture( 0, vsDesc );

			        SetAsset( 0, LAsset( vsDesc->GetName(), assetDescr, vsDesc->GetSamplerState() ) );

			        HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

                    UpdateDecoderState( m_decoderMode );

			        return true;
		        }
            }
        }
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultAVDecoderPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr		         DefaultAVDecoderPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultAVDecoderPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
IAudioChannelPtr                    DefaultAVDecoderPlugin::GetAudioChannel      () const
{
    return m_audioChannel;
}

// *************************************
// 
void                                DefaultAVDecoderPlugin::Update                      ( TimeType t )
{
   	BasePlugin::Update( t );
    m_decoderMode =  m_decoderModeParam->Evaluate();

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, "txMat" );

    MarkOffsetChanges();

	HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
	if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) )
	{
		InitVertexAttributesChannel();
	}
	HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    UploadVideoFrame();
    UploadAudioFrame();
    UpdateDecoder();

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void									DefaultAVDecoderPlugin::InitVertexAttributesChannel		()
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
			auto uvs = new model::Float2AttributeChannel( desc, DefaultAVDecoderPluginDesc::TextureName(), true );
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
void                                DefaultAVDecoderPlugin::UpdateDecoder  ()
{
    if( m_decoder )
    {
        //order matters - update offset first then mode
        auto offset = m_offsetParam->Evaluate();

        auto decoderModeTime = m_decoderModeParam->GetLocalEvaluationTime();

        //edge case - loop
        if( ( m_prevDecoderModeTime > decoderModeTime ) && ( m_decoderMode == DecoderMode::PLAY ) )
        {
            m_decoder->Play();
            m_prevOffsetCounter = 0;
        }
        m_prevDecoderModeTime = decoderModeTime;

        auto offsetTime = m_offsetParam->GetLocalEvaluationTime();
        if( ( m_prevOffsetCounter != offset[ 1 ] ) || ( m_prevOffsetTime > offsetTime ) )
        {
            //edge case - eof
            if( m_decoderMode == DecoderMode::PLAY )
            {
                m_decoder->Play();
                m_decoder->Seek( offset[ 0 ] );
            }
            else
            {
                m_decoder->Seek( offset[ 0 ] );
                m_decoder->FlushBuffers();
            }

            m_prevOffsetCounter = offset[ 1 ];
        }

        if( ParameterChanged( PARAM::DECODER_STATE ) )
        {
            UpdateDecoderState( m_decoderMode );
        }

        // handle perfect loops
        auto loopEnabled = m_loopEnabledParam->Evaluate();
        auto loopCount = m_loopCountParam->Evaluate();
        if( ParameterChanged( PARAM::LOOP_COUNT ) )
        {
            if( loopCount == 0 )
            {
                loopCount = std::numeric_limits< Int32 >::max(); // set 'infinite' loop
            }
            m_loopCount = loopCount;
        }

        if( loopEnabled && m_decoder->IsEOF() && m_loopCount > 1 )
        {
            m_decoder->Seek( 0.f );
            m_loopCount--;
        }

        // send event on video finished
        if( !m_isFinished && m_decoder->IsFinished() && m_assetDesc )
        {
            auto evt = std::make_shared< VideoDecoderEvent >();
            evt->AssetPath = m_assetDesc->GetStreamPath();
            evt->EventCommand = VideoDecoderEvent::Command::HasFinished;
            JsonSerializeObject ser;
            evt->Serialize( ser );
            SendResponse( ser, SEND_BROADCAST_EVENT, 0 );
            m_isFinished = true;
        }
    }
}

// *************************************
//
void                                DefaultAVDecoderPlugin::UpdateDecoderState   ( DecoderMode mode )
{
    switch( mode )
    {
        case DecoderMode::PLAY:
            m_decoder->Play();
            m_isFinished = false;
            break;
        case DecoderMode::STOP:
            m_decoder->Stop(); break;
        case DecoderMode::PAUSE:
            m_decoder->Pause(); break;
    }
}

// *************************************
//
void                                DefaultAVDecoderPlugin::UploadVideoFrame   ()
{
    //update texture with video data
    AVMediaData mediaData;
	if( m_decoder->GetVideoMediaData( mediaData ) )
	{
	    std::static_pointer_cast< DefaultVideoStreamDescriptor >( m_psc->GetTexturesDataImpl()->GetTexture( 0 ) )->SetBits( mediaData.frameData );
	}
}

// *************************************
//
void                                DefaultAVDecoderPlugin::UploadAudioFrame   ()
{
    //update audio data
    AVMediaData mediaData;
    if( m_decoder->GetAudioMediaData( mediaData ) )
	{
        m_audioChannel->PushPacket( mediaData.frameData );
	}
}

// *************************************
//
void                                DefaultAVDecoderPlugin::MarkOffsetChanges  ()
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