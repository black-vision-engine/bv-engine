#include "DefaultAlphaMaskPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAnimationPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGradientPlugin.h"
#include "DefaultTransformPlugin.h"

namespace bv { namespace model {

namespace {
    const float pixelsPerUnitUVSpace = 540.f; //FIXME: ultra hacking
}

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultAlphaMaskPluginDesc::DefaultAlphaMaskPluginDesc                      ()
    : BasePluginDescriptor( UID(), "alpha_mask", "am" )
{
}

// *******************************
//
IPluginPtr              DefaultAlphaMaskPluginDesc::CreatePlugin            ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultAlphaMaskPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultAlphaMaskPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txAlphaMat", timeEvaluator );

    ParamFloatPtr  paramWrapModeX     = ParametersFactory::CreateParameterFloat( "wrapModeX", timeEvaluator );
    ParamFloatPtr  paramWrapModeY     = ParametersFactory::CreateParameterFloat( "wrapModeY", timeEvaluator );
    ParamFloatPtr  paramFilteringMode = ParametersFactory::CreateParameterFloat( "filteringMode", timeEvaluator );
    ParamFloatPtr  paramAttachMode    = ParametersFactory::CreateParameterFloat( "attachmentMode", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->AddParameter( paramWrapModeX );
    psModel->AddParameter( paramWrapModeY );
    psModel->AddParameter( paramFilteringMode );
    psModel->AddParameter( paramAttachMode );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

    //FIXME: integer parmeters should be used here
    paramWrapModeX->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    paramWrapModeY->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    paramFilteringMode->SetVal( (float) TextureFilteringMode::TFM_LINEAR, TimeType( 0.f ) );
    paramAttachMode->SetVal( (float) TextureAttachmentMode::MM_ATTACHED, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultAlphaMaskPluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return false;
    }
/*
    auto  vac = plugin->GetVertexAttributesChannel();
    if ( vac == nullptr )
    {
        return false;
    }
    */
//    auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
//    if ( numChannels != 1 && numChannels != 2 ) //vertex attribute + optional texture
//    {
//        return false;
//    }

    auto uid = plugin->GetTypeUid();

	//if ( uid != DefaultColorPluginDesc::UID() && uid != DefaultTexturePluginDesc::UID() && uid != DefaultTextPluginDesc::UID() && uid != DefaultAnimationPluginDesc::UID() && uid != DefaultGradientPluginDesc::UID() ) // FUNKED for serialization
 //   {
 //       return false;
 //   }

    return true;
}

// *******************************
//
std::string             DefaultAlphaMaskPluginDesc::UID                     ()
{
    return "DEFAULT_ALPHA_MASK";
}

// *******************************
//
std::string             DefaultAlphaMaskPluginDesc::TextureName             ()
{
    return "AlphaTex0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void								DefaultAlphaMaskPlugin::SetPrevPlugin               ( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
    
    if( prev == nullptr )
        return;

    //FIXME: The hackiest of it all - added registered parameters to pass on to the engine - ten kod jest przestraszny i wykurwiscie niefajny
    if( prev->GetTypeUid() == DefaultColorPluginDesc::UID() )
    {
		std::vector< std::string > psEval;
		psEval.push_back( "color" );
		RegisterEvaluators( prev, std::vector< std::string >(), psEval );
    }
    else if( prev->GetTypeUid() == DefaultTexturePluginDesc::UID() || prev->GetTypeUid() == DefaultAnimationPluginDesc::UID() )
    {
		std::vector< std::string > vsEval, psEval;
		vsEval.push_back( "txMat" );
		psEval.push_back( "alpha" );
		RegisterEvaluators( prev, vsEval, psEval );
    }
    else if( prev->GetTypeUid() == DefaultTextPluginDesc::UID() )
    {
        std::vector< std::string > psEval;
		psEval.push_back( "alpha" );
		psEval.push_back( "color" );
		RegisterEvaluators( prev, std::vector< std::string >(), psEval );
    }
	else if( prev->GetTypeUid() == DefaultGradientPluginDesc::UID() )
    {
		std::vector< std::string > vsEval, psEval;
		vsEval.push_back( "txMat" );
		psEval.push_back( "color1" );
		psEval.push_back( "color2" );
		psEval.push_back( "point1" );
		psEval.push_back( "point2" );
		RegisterEvaluators( prev, vsEval, psEval );
    }

    InitAttributesChannel( prev );

    if( prev->GetTypeUid() == DefaultTexturePluginDesc::UID() || prev->GetTypeUid() == DefaultAnimationPluginDesc::UID() || prev->GetTypeUid() == DefaultTextPluginDesc::UID() )
    {
        //FIXME: set textures data from prev plugin to this plugin
        auto prev_psc = std::const_pointer_cast< ITexturesData >( prev->GetPixelShaderChannel()->GetTexturesData() );
        //FIXME: this line causes changes to Texture Plugin data via current pointer - quite shitty
        m_psc->OverrideTexturesData( std::static_pointer_cast< DefaultTexturesData >( prev_psc ) );
    }
}

// *************************************
// 
DefaultAlphaMaskPlugin::DefaultAlphaMaskPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_textureWidth( 0 )
    , m_textureHeight( 0 )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
    
	SetPrevPlugin( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    ctx->alphaCtx->blendEnabled = true;

    m_texturesData = m_psc->GetTexturesDataImpl();

    //Direct param state access (to bypass model querying)
    auto psModel = PixelShaderChannelModel();
    
    m_paramWrapModeX        = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "wrapModeX" ) );
    m_paramWrapModeY        = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "wrapModeY" ) );
    m_paramFilteringMode    = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "filteringMode" ) );
    m_paramAttachMode       = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "attachmentMode" ) );

    assert( m_paramWrapModeX );
    assert( m_paramWrapModeY );
    assert( m_paramFilteringMode );
    assert( m_paramAttachMode );

    auto wX = GetWrapModeX();
    auto wY = GetWrapModeY();
    auto fm = GetFilteringMode();
    auto am = GetAttachementMode();

    UpdateState( wX, wY, fm, am );
}

// *************************************
// 
DefaultAlphaMaskPlugin::~DefaultAlphaMaskPlugin         ()
{
}

// *************************************
// 
bool                        DefaultAlphaMaskPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
	auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 2 );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultAlphaMaskPluginDesc::TextureName() );

        //Alpha texture defaults
        txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
        txDesc->SetBorderColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
        txDesc->SetWrappingModeX( TextureWrappingMode::TWM_CLAMP_BORDER );
        txDesc->SetWrappingModeY( TextureWrappingMode::TWM_CLAMP_BORDER );
        txDesc->SetFilteringMode( TextureFilteringMode::TFM_LINEAR );

        if( txDesc != nullptr )
        {
			auto prevTypeUid = GetPrevPlugin()->GetTypeUid();
            if( prevTypeUid == DefaultColorPluginDesc::UID()
				|| prevTypeUid == DefaultAnimationPluginDesc::UID()
				|| prevTypeUid == DefaultGradientPluginDesc::UID() )
            {
                txData->SetTexture( 0, txDesc );
            }
            else if( prevTypeUid == DefaultTexturePluginDesc::UID() 
				|| prevTypeUid == DefaultTextPluginDesc::UID() )
            {
                assert( txData->GetTextures().size() >= 1 ); //FIXME: texture plugin is supposed to be added first
				txData->SetTexture( 1, txDesc );
            }
            else
            {
                assert( false );
            }

            m_textureWidth = txDesc->GetWidth();
            m_textureHeight = txDesc->GetHeight();

			RecalculateUVChannel();

            return true;
        }
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultAlphaMaskPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultAlphaMaskPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultAlphaMaskPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultAlphaMaskPlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused variable
    m_paramValModel->Update();

    auto attachmentMode = GetAttachementMode();
    auto wX = GetWrapModeX();
    auto wY = GetWrapModeY();
    auto fm = GetFilteringMode();

	if( m_vaChannel )
	{
		bool hasPrevVAC = m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel();
		if ( hasPrevVAC && m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() 
			|| StateChanged( wX, wY, fm, attachmentMode ) )
		{
			RecalculateUVChannel();
			UpdateState( wX, wY, fm, attachmentMode );
			m_vaChannel->SetNeedsAttributesUpdate( true );
		}
		else
		{
			m_vaChannel->SetNeedsAttributesUpdate( false );
		}

		if( hasPrevVAC && m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() )
		{
			m_vaChannel->ClearAll();
			InitAttributesChannel( m_prevPlugin );
			m_vaChannel->SetNeedsTopologyUpdate( true );
			m_vaChannel->SetNeedsAttributesUpdate( false ); // FIXME: very ugly hack this is
		}
		else
		{
			m_vaChannel->SetNeedsTopologyUpdate( false );
		}
	}

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void DefaultAlphaMaskPlugin::InitAttributesChannel( IPluginConstPtr prev )
{
	if( !( prev && prev->GetVertexAttributesChannel() ) )
	{
		m_vaChannel = nullptr;
		return;
	}

    auto prevGeomChannel = prev->GetVertexAttributesChannel();
	auto prevCC = prevGeomChannel->GetComponents();

//recreate vachannel ->
	VertexAttributesChannelDescriptor vaChannelDesc;
	auto prevCompChannels = prevCC[ 0 ]->GetAttributeChannels(); //FIXME: is it possible that CC is empty?
    for( auto prevCompCh : prevCompChannels )
    {
        auto prevCompChDesc = prevCompCh->GetDescriptor();
        vaChannelDesc.AddAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
    }

    //add alpha mask texture desc
	vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

	m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
//<- recreate vachannel


    auto desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    for( unsigned int i = 0; i < prevCC.size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();
        auto prevCompChannels = std::static_pointer_cast< const model::ConnectedComponent >( prevGeomChannel->GetComponents()[ i ] )->GetAttributeChannelsPtr();
		
        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

		//add alpha mask uv channel
		connComp->AddAttributeChannel( std::make_shared< Float2AttributeChannel >( desc, DefaultAlphaMaskPluginDesc::TextureName(), true ) );

        m_vaChannel->AddConnectedComponent( connComp );
    }

	RecalculateUVChannel();
}

// *************************************
//
void     DefaultAlphaMaskPlugin::RecalculateUVChannel         ()
{
	//FIXME: only one texture - convex hull calculations
    float minX = 100000.0f, minY = 100000.0f;
    float maxX = 0.0f, maxY = 0.0f;
    float pixelsPerUnitUVSpace = 1080.f;

	float txWidth = m_textureWidth > 0 ? ( float )m_textureWidth : 1920.f;
	float txHeight = m_textureHeight > 0 ? ( float )m_textureHeight : 1080.f;

	auto cc = m_vaChannel->GetComponents();
	for( unsigned int i = 0; i < cc.size(); ++i )
	{
		auto compChannels = cc[ i ]->GetAttributeChannels();
		auto posChannel = AttributeChannel::GetAttrChannel( compChannels, AttributeSemantic::AS_POSITION );
		if( posChannel )
		{
			auto pos = std::static_pointer_cast< Float3AttributeChannel >( posChannel )->GetVertices();
			for( unsigned int j = 0; j < posChannel->GetNumEntries(); ++j )
			{
				minX = std::min( minX, pos[ j ].x );
				minY = std::min( minY, pos[ j ].y );
				maxX = std::max( maxX, pos[ j ].x );
				maxY = std::max( maxY, pos[ j ].y );
			}
		}
	}

	for( unsigned int i = 0; i < cc.size(); ++i )
	{
		auto compChannels = cc[ i ]->GetAttributeChannels();
		auto posChannel = AttributeChannel::GetAttrChannel( compChannels, AttributeSemantic::AS_POSITION );
		auto uvChannel = AttributeChannel::GetAttrChannel( compChannels, AttributeSemantic::AS_TEXCOORD, 1 );
		
		if( posChannel && uvChannel )
		{
			auto pos = std::static_pointer_cast< Float3AttributeChannel >( posChannel );
			auto uvs = std::static_pointer_cast< Float2AttributeChannel >( uvChannel );

			auto & uvVerts = uvs->GetVertices();
			if( uvVerts.size() < posChannel->GetNumEntries() )
			{
				uvVerts.resize( posChannel->GetNumEntries() );
				m_vaChannel->SetNeedsTopologyUpdate( true );
			}

			auto & posVerts = pos->GetVertices();
			for( unsigned int j = 0; j < posChannel->GetNumEntries(); ++j )
			{
				uvVerts[ j ] = glm::vec2( pixelsPerUnitUVSpace * ( posVerts[ j ].x - minX ) / txWidth, 
					pixelsPerUnitUVSpace * ( posVerts[ j ].y - minY ) / txHeight );
			}
		}
	}

	m_vaChannel->SetNeedsAttributesUpdate( true );
}

// *************************************
//
void					DefaultAlphaMaskPlugin::RegisterEvaluators			( IPluginPtr prev, const std::vector< std::string > & vsEvaluators, const std::vector< std::string > & psEvaluators )
{
	for( auto paramName : vsEvaluators )
	{
		assert( prev->GetParameter( paramName ) != nullptr );
	}
        
	if( prev->GetPluginParamValModel()->GetVertexShaderChannelModel() )
	{
		auto evaluatorsv = prev->GetPluginParamValModel()->GetVertexShaderChannelModel()->GetEvaluators();
		for( auto & eval : evaluatorsv )
		{
			for( auto & paramName : vsEvaluators )
			{
				if( eval->GetParameter( paramName ) )
				{
					//FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak), robione podwojnie updaty, tego typu duperele
					std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetVertexShaderChannelModel() )->RegisterAll( eval );
				}
			}
		}
	}

	for( auto paramName : psEvaluators )
	{
		assert( prev->GetParameter( paramName ) != nullptr );
	}

	if( prev->GetPluginParamValModel()->GetPixelShaderChannelModel() )
	{
		auto evaluatorsp = prev->GetPluginParamValModel()->GetPixelShaderChannelModel()->GetEvaluators();
		for( auto & eval : evaluatorsp )
		{
			for( auto & paramName : psEvaluators )
			{
				if( eval->GetParameter( paramName ) )
				{
					//FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak), robione podwojnie updaty, tego typu duperele
					std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( eval );
				}
			}
		}
	}
}

namespace {

// *************************************
// FIXME: implement int parameters and bool parameters
template< typename EnumClassType >
inline EnumClassType EvaluateAsInt( ParamFloat * param )
{
    int val = int( param->Evaluate() );

    return EnumClassType( val );
}

// *************************************
// FIXME: implement int parameters and bool parameters
template< typename EnumClassType >
inline EnumClassType EvaluateAsInt( ParamFloatPtr param )
{
    int val = int( param->Evaluate() );

    return EnumClassType( val );
}

} //anonymous

// *************************************
// 
TextureWrappingMode                         DefaultAlphaMaskPlugin::GetWrapModeX        () const
{
    return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeX );
}

// *************************************
// 
TextureWrappingMode                         DefaultAlphaMaskPlugin::GetWrapModeY        () const
{
    return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeY );
}

// *************************************
// 
TextureFilteringMode                        DefaultAlphaMaskPlugin::GetFilteringMode    () const
{
    return EvaluateAsInt< TextureFilteringMode >( m_paramFilteringMode );
}

// *************************************
// 
TextureAttachmentMode                       DefaultAlphaMaskPlugin::GetAttachementMode  () const
{
    return EvaluateAsInt< TextureAttachmentMode >( m_paramAttachMode );
}

// *************************************
// 
bool                                        DefaultAlphaMaskPlugin::StateChanged        ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am ) const
{
    return wmX != m_lastTextureWrapModeX || wmY != m_lastTextureWrapModeY || fm != m_lastTextureFilteringMode || am != m_lastTextureAttachMode;
}

// *************************************
// 
void                                        DefaultAlphaMaskPlugin::UpdateState         ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am )
{
    m_lastTextureWrapModeX      = wmX;
    m_lastTextureWrapModeY      = wmY;
    m_lastTextureFilteringMode  = fm;
    m_lastTextureAttachMode     = am;
}

// *************************************
// 
SizeType									DefaultAlphaMaskPlugin::GetAlphaTextureWidth    () const
{
    return m_textureWidth;
}

// *************************************
//
SizeType                                     DefaultAlphaMaskPlugin::GetAlphaTextureHeight   () const
{
    return m_textureHeight;
}

} // model
} // bv
