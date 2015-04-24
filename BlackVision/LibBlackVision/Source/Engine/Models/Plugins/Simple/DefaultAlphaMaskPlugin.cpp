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

	if ( uid != DefaultColorPluginDesc::UID() && uid != DefaultTexturePluginDesc::UID() && uid != DefaultTextPluginDesc::UID() && uid != DefaultAnimationPluginDesc::UID() && uid != DefaultGradientPluginDesc::UID() )
    {
        return false;
    }

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
std::string             DefaultAlphaMaskPluginDesc::VertexShaderSource      ()
{
    return "Assets/Shaders/Deprecated/dummy.vert"; //FIXME: deprecated
}

// *******************************
//
std::string             DefaultAlphaMaskPluginDesc::PixelShaderSource       ()
{
    return "Assets/Shaders/Deprecated/dummy.frag"; //FIXME: deprecated
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
DefaultAlphaMaskPlugin::DefaultAlphaMaskPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_textureWidth( 0 )
    , m_textureHeight( 0 )
{
    //FIXME: The hackiest of it all - added registered parameters to pass on to the engine - ten kod jest przestraszny i wykurwiscie niefajny
    if( prev->GetTypeUid() == DefaultColorPluginDesc::UID() )
    {
        assert( prev->GetParameter( "color" ) != nullptr );

        auto evaluators = prev->GetPluginParamValModel()->GetPixelShaderChannelModel()->GetEvaluators();
        for( unsigned int i = 0; i < evaluators.size(); ++i )
        {
            auto colorParam = evaluators[ i ]->GetParameter( "color" );
            if( colorParam != nullptr )
            {
                //FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak), robione podwojnie updaty, tego typu duperele
                std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( evaluators[ i ] );
                break;
            }
        }
    }
    else if( prev->GetTypeUid() == DefaultTexturePluginDesc::UID() || prev->GetTypeUid() == DefaultAnimationPluginDesc::UID() )
    {
        assert( prev->GetParameter( "alpha" ) != nullptr );
        
        auto evaluatorsp = prev->GetPluginParamValModel()->GetPixelShaderChannelModel()->GetEvaluators();
        for( unsigned int i = 0; i < evaluatorsp.size(); ++i )
        {
            auto colorParam = evaluatorsp[ i ]->GetParameter( "alpha" );
            if( colorParam != nullptr )
            {
                //FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak), robione podwojnie updaty, tego typu duperele
                std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( evaluatorsp[ i ] );
                break;
            }
        }

        assert( prev->GetParameter( "txMat" ) != nullptr );
        
        auto evaluatorsv = prev->GetPluginParamValModel()->GetVertexShaderChannelModel()->GetEvaluators();
        for( unsigned int i = 0; i < evaluatorsv.size(); ++i )
        {
            auto colorParam = evaluatorsv[ i ]->GetParameter( "txMat" );
            if( colorParam != nullptr )
            {
                //FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak), robione podwojnie updaty, tego typu duperele
                std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetVertexShaderChannelModel() )->RegisterAll( evaluatorsv[ i ] );
                break;
            }
        }
    }
    else if( prev->GetTypeUid() == DefaultTextPluginDesc::UID() )
    {
        assert( prev->GetParameter( "alpha" ) != nullptr );
        assert( prev->GetParameter( "color" ) != nullptr );
        
        auto evaluatorsp = prev->GetPluginParamValModel()->GetPixelShaderChannelModel()->GetEvaluators();
        for( unsigned int i = 0; i < evaluatorsp.size(); ++i )
        {
            auto colorParam = evaluatorsp[ i ]->GetParameter( "alpha" );
            if( colorParam != nullptr )
            {
                //FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak), robione podwojnie updaty, tego typu duperele
                std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( evaluatorsp[ i ] );
            }

            auto colorParami = evaluatorsp[ i ]->GetParameter( "color" );
            if( colorParami != nullptr )
            {
                //FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak), robione podwojnie updaty, tego typu duperele
                std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( evaluatorsp[ i ] );
            }
        }
    }

    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultAlphaMaskPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultAlphaMaskPluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );

    InitAttributesChannel( prev );

    if( prev->GetTypeUid() == DefaultTexturePluginDesc::UID() || prev->GetTypeUid() == DefaultAnimationPluginDesc::UID() || prev->GetTypeUid() == DefaultTextPluginDesc::UID() )
    {
        //FIXME: set textures data from prev plugin to this plugin
        auto prev_psc = std::const_pointer_cast< ITexturesData >( prev->GetPixelShaderChannel()->GetTexturesData() );
        //FIXME: this line causes changes to Texture Plugin data via current pointer - quite shitty
        m_psc->OverrideTexturesData( std::static_pointer_cast< DefaultTexturesData >( prev_psc ) );
    }

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
            if( GetPrevPlugin()->GetTypeUid() == DefaultColorPluginDesc::UID() )
            {
                if( txData->GetTextures().size() == 0 )
                {
                    txData->AddTexture( txDesc );
                }
                else
                {
                    assert( txData->GetTextures().size() == 1 );

                    txData->SetTexture( 0, txDesc );
                }
            }
            else if( GetPrevPlugin()->GetTypeUid() == DefaultTexturePluginDesc::UID() )
            {
                assert( txData->GetTextures().size() >= 1 ); //FIXME: texture plugin is supposed to be added first

                if( txData->GetTextures().size() == 1 )
                {
                    txData->AddTexture( txDesc );
                }
                else
                {
                    assert( txData->GetTextures().size() == 2 );

                    txData->SetTexture( 1, txDesc );
                }
            }
            else if( GetPrevPlugin()->GetTypeUid() == DefaultAnimationPluginDesc::UID() )
            {
                assert( txData->GetTextures().size() <= 1 ); //FIXME: texture plugin is supposed to be added first

                if( txData->GetTextures().size() == 0 )
                {
                    txData->AddTexture( txDesc );
                }
                else
                {
                    assert( txData->GetTextures().size() == 1 );

                    txData->SetTexture( 0, txDesc );
                }
            }
            else if( GetPrevPlugin()->GetTypeUid() == DefaultTextPluginDesc::UID() )
            {
                assert( txData->GetTextures().size() >= 1 ); //FIXME: text plugin is supposed to be added first

                if( txData->GetTextures().size() == 1 )
                {
                    txData->AddTexture( txDesc );
                }
                else
                {
                    assert( txData->GetTextures().size() == 2 );

                    txData->SetTexture( 1, txDesc );
                }
            }
            else
            {
                assert( false );
            }

            m_textureWidth = txDesc->GetWidth();
            m_textureHeight = txDesc->GetHeight();

            auto prev = GetPrevPlugin();

            RecalculateAttrChannel();

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

    if( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
    {
        RecalculateAttrChannel();
    }

    if( m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() ) //FIXME: additionalna hackierka
    {
        if( m_vaChannel != nullptr )
        {
            m_vaChannel->ClearAll();
        }
        //m_vaChannel = nullptr;

        assert( m_prevPlugin->GetTypeUid() == DefaultTextPluginDesc::UID() );

        InitAttributesChannel( m_prevPlugin );

        //FIXME: very fucking hack - treats text connected components as FREE uv mapping
        //FIXME: only one texture - convex hull calculations
        float minX = 100000.0f, minY = 100000.0f;
        float maxX = 0.0f, maxY = 0.0f;
            
        float txWidth = 1920.f;
        float txHeight = 1080.f;

        if( m_textureWidth > 0 )
        {
            txWidth = (float) m_textureWidth;
        }

        if( m_textureHeight > 0 )
        {
            txHeight = (float) m_textureHeight;
        }

        //Calculate min max
        for( unsigned int i = 0; i < m_vaChannel->GetComponents().size(); ++i )
        {
            auto connComp = m_vaChannel->GetConnectedComponent( i );
            auto compChannels = connComp->GetAttributeChannels();

            if( auto posChannel = AttributeChannel::GetPositionChannel( compChannels ) )
            {
                if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, m_texCoordChannelIndex ) )
                {
                    auto & verts  = std::dynamic_pointer_cast< Float3AttributeChannel >( posChannel )->GetVertices();

                    for( unsigned int j = 0; j < verts.size(); ++j )
                    {
                        minX = std::min( minX, verts[ j ].x );
                        minY = std::min( minY, verts[ j ].y );
                        maxX = std::max( maxX, verts[ j ].x );
                        maxY = std::max( maxY, verts[ j ].y );
                    }
                }
            }
        }

        //printf( "************** RECALCULATED MAPPING *****************\n" );
        //Recalculate uv mapping
        for( unsigned int i = 0; i < m_vaChannel->GetComponents().size(); ++i )
        {
            auto connComp = m_vaChannel->GetConnectedComponent( i );
            auto compChannels = connComp->GetAttributeChannels();

            if( auto posChannel = AttributeChannel::GetPositionChannel( compChannels ) )
            {
                //printf( "CComp %d with texture (%d, %d) \n", i, m_textureWidth, m_textureHeight );
                if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, m_texCoordChannelIndex ) )
                {
                    auto & verts  = std::dynamic_pointer_cast< Float3AttributeChannel >( posChannel )->GetVertices();
                    auto & uvs    = std::dynamic_pointer_cast< Float2AttributeChannel >( uvChannel )->GetVertices();

                    for( unsigned int j = 0; j < verts.size(); ++j )
                    {
                        float u0 = verts[ j ].x;
                        float v0 = verts[ j ].y;

                        float u1 = pixelsPerUnitUVSpace * ( verts[ j ].x - minX ) / txWidth;
                        float v1 = pixelsPerUnitUVSpace * ( verts[ j ].y - minY ) / txHeight;

                        //float tmp[] = { u0, v0, u1, v1 };

                        uvs[ j ].x = u1;
                        uvs[ j ].y = v1;

                        printf( "   Vertex %d (%.3f, %.3f, %.3f) -> (%.3f, %3f), (%.3f, %3f)\n", j, verts[ j ].x, verts[ j ].y, verts[ j ].z, u0, v0, u1, v1 );
                    }
                }
            }
        }
    }
    //FIXME: olewamy MM_FREE w tym pluginie (i tak na razie nie jest nigdzie uzywane)
#if 0
    if( attachmentMode == TextureAttachmentMode::MM_FREE )
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
#endif //if 0

    auto wX = GetWrapModeX();
    auto wY = GetWrapModeY();
    auto fm = GetFilteringMode();

    if( m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() )
    {
        m_vaChannel->SetNeedsTopologyUpdate( true );
    }
    else
    {
        if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() || StateChanged( wX, wY, fm, attachmentMode ) )
        {
            UpdateState( wX, wY, fm, attachmentMode );
            m_vaChannel->SetNeedsAttributesUpdate( true );
        }
        else
        {
            m_vaChannel->SetNeedsAttributesUpdate( false );
        }

        m_vaChannel->SetNeedsTopologyUpdate( false );
    }

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void DefaultAlphaMaskPlugin::InitAttributesChannel( IPluginConstPtr prev )
{
    auto prevGeomChannel = prev->GetVertexAttributesChannel();

    if( prevGeomChannel == nullptr ) //FIXME: hackierka
    {
        assert( prev->GetTypeUid() == DefaultTextPluginDesc::UID() );

        return;
    }

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

        if( m_vaChannel == nullptr || m_vaChannel->NeedsInitialization() )
        {
            for( auto prevCompCh : prevCompChannels )
            {
                auto prevCompChDesc = prevCompCh->GetDescriptor();
                vaChannelDesc.AddAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
            }

            m_texCoordChannelIndex = vaChannelDesc.GetNumVertexChannels();

            //Only one texture
            vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            if( m_vaChannel == nullptr )
            {
                m_vaChannel = VertexAttributesChannelPtr( new VertexAttributesChannel( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() ) );
            }

            m_vaChannel->Initialize( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
        }

        //FIXME: only one texture - convex hull calculations
        float minX = 100000.0f, minY = 100000.0f;
        float maxX = 0.0f, maxY = 0.0f;
        float pixelsPerUnitUVSpace = 1080.f;
            
        float txWidth = 1920.f;
        float txHeight = 1080.f;

        if( m_textureWidth > 0 )
        {
            txWidth = (float) m_textureWidth;
        }

        if( m_textureHeight > 0 )
        {
            txHeight = (float) m_textureHeight;
        }

        //FIXME: hackish as hell positioning (assuming implicit FullHD)
        //convex hull - make sure that prevCompChannels[ 0 ] is indeed a positional channel
        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );

            minX = std::min( minX, pos[ j ].x );
            minY = std::min( minY, pos[ j ].y );
            maxX = std::max( maxX, pos[ j ].x );
            maxY = std::max( maxY, pos[ j ].y );
        }

        auto verTexAttrChannel = new model::Float2AttributeChannel( desc, DefaultAlphaMaskPluginDesc::TextureName(), true );

        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );
            //verTexAttrChannel->AddAttribute( glm::vec2( pixelsPerUnitUVSpace * ( pos[ j ].x - minX ) / ( maxX - minX ) / txWidth, pixelsPerUnitUVSpace * ( pos[ j ].y - minY ) / ( maxY - minY ) / txHeight ) );
            verTexAttrChannel->AddAttribute( glm::vec2( pixelsPerUnitUVSpace * ( pos[ j ].x - minX ) / txWidth, pixelsPerUnitUVSpace * ( pos[ j ].y - minY ) / txHeight ) );
        }

        connComp->AddAttributeChannel( AttributeChannelPtr( verTexAttrChannel ) );

        m_vaChannel->AddConnectedComponent( connComp );
    }
}

// *************************************
//
void     DefaultAlphaMaskPlugin::RecalculateAttrChannel         ()
{
    auto prevGeomChannel = GetPrevPlugin()->GetVertexAttributesChannel();

    if( prevGeomChannel == nullptr || m_vaChannel == nullptr ) //FIXME: hackierka
    {
        assert( GetPrevPlugin()->GetTypeUid() == DefaultTextPluginDesc::UID() );

        return;
    }

    assert( m_vaChannel != nullptr );
    assert( m_textureWidth > 0 );
    assert( m_textureHeight > 0 );

    for( unsigned int i = 0; i < m_vaChannel->GetComponents().size(); ++i )
    {
        auto cc = m_vaChannel->GetConnectedComponent( i );
        auto ccAttrChannels = cc->GetAttributeChannelsPtr();
        auto posChannel = ccAttrChannels.front(); //FIXME: implicit assumption that first channel contains positional data !!!
        auto uvChannel = ccAttrChannels.back();

        assert( uvChannel == ccAttrChannels[ m_texCoordChannelIndex ] );

        //FIXME: only one texture - convex hull calculations
        float minX = 100000.0f, minY = 100000.0f;
        float maxX = 0.0f, maxY = 0.0f;

        float txWidth = (float) m_textureWidth;
        float txHeight = (float) m_textureHeight;

        //FIXME: hackish as hell positioning (assuming implicit FullHD)
        //convex hull - make sure that prevCompChannels[ 0 ] is indeed a positional channel
        for( unsigned int j = 0; j < posChannel->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( posChannel->GetData() );

            minX = std::min( minX, pos[ j ].x );
            minY = std::min( minY, pos[ j ].y );
            maxX = std::max( maxX, pos[ j ].x );
            maxY = std::max( maxY, pos[ j ].y );
        }

        Float2AttributeChannelPtr uv = std::static_pointer_cast< Float2AttributeChannel >( uvChannel ); //FIXME: poor, poor, poor
        auto & uvVec = uv->GetVertices();

        for( unsigned int j = 0; j < uvVec.size(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( posChannel->GetData() );

            //uvVec[ j ].x = pixelsPerUnitUVSpace * ( pos[ j ].x - minX ) / ( maxX - minX ) / txWidth;
            //uvVec[ j ].y = pixelsPerUnitUVSpace * ( pos[ j ].y - minY ) / ( maxY - minY ) / txHeight;
            uvVec[ j ].x = pixelsPerUnitUVSpace * ( pos[ j ].x - minX ) / txWidth;
            uvVec[ j ].y = pixelsPerUnitUVSpace * ( pos[ j ].y - minY ) / txHeight;

            //printf( "   Vertex %d (%.3f, %.3f, %.3f) -> (%.3f, %3f)\n", j, pos[ j ].x, pos[ j ].y, pos[ j ].z,uvVec[ j ].x, uvVec[ j ].y );
        }
    }

    m_vaChannel->SetNeedsAttributesUpdate( true );
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
