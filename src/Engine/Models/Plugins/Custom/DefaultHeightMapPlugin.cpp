#include "DefaultHeightMapPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultHeightMapPluginDesc::DefaultHeightMapPluginDesc                          ()
    : BasePluginDescriptor( UID(), "height map" )
{
}

// *******************************
//
IPluginPtr              DefaultHeightMapPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultHeightMapPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultHeightMapPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    //SimpleFloatEvaluatorPtr     centerXEvaluator                = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "centerX", timeEvaluator );
    //SimpleFloatEvaluatorPtr     centerYEvaluator                = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "centerY", timeEvaluator );
    
    SimpleVec2EvaluatorPtr      scaleEvaluator                  = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "scale", timeEvaluator );

    SimpleFloatEvaluatorPtr     startDistInMetersEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "startDistInMeters", timeEvaluator );
    SimpleFloatEvaluatorPtr     endDistInMetersEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "endDistInMeters", timeEvaluator );

    SimpleFloatEvaluatorPtr     curDistanceInMetersEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "curDistanceInMeters", timeEvaluator );
    SimpleFloatEvaluatorPtr     totalDistanceInMetersEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "totalDistanceInMeters", timeEvaluator );

    SimpleFloatEvaluatorPtr     hmOffsetYInPixelsEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "hmOffsetYInPixels", timeEvaluator );

    SimpleFloatEvaluatorPtr     hmMaxHeightValEvaluator         = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "hmMaxHeightValue", timeEvaluator );
    SimpleFloatEvaluatorPtr     hmMinHeightValEvaluator         = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "hmMinHeightValue", timeEvaluator );
    SimpleFloatEvaluatorPtr     hmGroundLevelHeightEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "hmGroundLevelHeight", timeEvaluator );
    SimpleFloatEvaluatorPtr     hmHeightScaleEvaluator          = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "hmHeightScale", timeEvaluator );
    SimpleFloatEvaluatorPtr     hmPowFactorEvaluator            = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "hmPowFactor", timeEvaluator );

    SimpleVec2EvaluatorPtr      hmShadowOffsetInPixelsEvaluator = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "hmShadowOffsetInPixels", timeEvaluator );
    SimpleVec4EvaluatorPtr      hmShadowColorEvaluator          = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "hmShadowColor", timeEvaluator );

    SimpleVec4EvaluatorPtr      hmOutlineInnedColorEvaluator    = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "hmOutlineInnedColor", timeEvaluator );
    SimpleVec4EvaluatorPtr      hmOutlineEdgeColorEvaluator     = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "hmOutlineEdgeColor", timeEvaluator );

    SimpleFloatEvaluatorPtr     coveredDistShowFactorEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "coveredDistShowFactor", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( scaleEvaluator );
    vsModel->RegisterAll( totalDistanceInMetersEvaluator );
    vsModel->RegisterAll( curDistanceInMetersEvaluator );
    vsModel->RegisterAll( startDistInMetersEvaluator );
    vsModel->RegisterAll( endDistInMetersEvaluator );

    psModel->RegisterAll( hmOffsetYInPixelsEvaluator );
    psModel->RegisterAll( coveredDistShowFactorEvaluator );
    psModel->RegisterAll( hmMaxHeightValEvaluator );
    psModel->RegisterAll( hmMinHeightValEvaluator );
    psModel->RegisterAll( hmHeightScaleEvaluator );
    psModel->RegisterAll( hmPowFactorEvaluator );
    psModel->RegisterAll( hmGroundLevelHeightEvaluator );
    psModel->RegisterAll( hmShadowOffsetInPixelsEvaluator );
    psModel->RegisterAll( hmShadowColorEvaluator );
    psModel->RegisterAll( hmOutlineInnedColorEvaluator );
    psModel->RegisterAll( hmOutlineEdgeColorEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //FIXME: Set default values
    //centerXEvaluator->Parameter()->SetVal( 0.5f, TimeType( 0.0 ) );
    //centerYEvaluator->Parameter()->SetVal( 0.5f, TimeType( 0.0 ) );

    hmOffsetYInPixelsEvaluator->Parameter()->SetVal( 98.0f, TimeType( 0.0 ) );

    hmMaxHeightValEvaluator->Parameter()->SetVal( 1009.1532f, TimeType( 0.0 ) );
    hmMinHeightValEvaluator->Parameter()->SetVal( 681.1941f, TimeType( 0.0 ) );
    hmHeightScaleEvaluator->Parameter()->SetVal( 194.f / 1080.f, TimeType( 0.0 ) );
    hmPowFactorEvaluator->Parameter()->SetVal( 1.0f, TimeType( 0.0 ) );
    hmGroundLevelHeightEvaluator->Parameter()->SetVal( 300.f, TimeType( 0.0 ) );

    hmShadowOffsetInPixelsEvaluator->Parameter()->SetVal( glm::vec2( -6.f, 6.f ), TimeType( 0.0 ) );
    hmShadowColorEvaluator->Parameter()->SetVal( glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );

    hmOutlineInnedColorEvaluator->Parameter()->SetVal( glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );
    hmOutlineEdgeColorEvaluator->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );

    scaleEvaluator->Parameter()->SetVal( glm::vec2( 1.f, 1.f ), TimeType( 0.0 ) );

    curDistanceInMetersEvaluator->Parameter()->SetVal( 0.0f, TimeType( 0.0 ) );
    startDistInMetersEvaluator->Parameter()->SetVal( 0.0f, TimeType( 0.0 ) );
    endDistInMetersEvaluator->Parameter()->SetVal( 174227.f, TimeType( 0.0 ) );
    totalDistanceInMetersEvaluator->Parameter()->SetVal( 174227.f, TimeType( 0.0 ) );

    //snapToPixelWeightEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );

    coveredDistShowFactorEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) ); //FIXME: ustawiane inne zmienne na bazie tych wartosci powinny byc ustawiane dla czasu zero (w innych pluginach)

    return model;
}

// *******************************
//
bool                   DefaultHeightMapPluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr || plugin->GetTypeUid() != DefaultRectPluginDesc::UID() )
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string             DefaultHeightMapPluginDesc::UID                     ()
{
    return "DEFAULT_HEIGHT_MAP";
}

// *******************************
//
std::string             DefaultHeightMapPluginDesc::HeightMapTextureName    ()
{
    return "HeightMapTex";
}

// *******************************
//
std::string             DefaultHeightMapPluginDesc::HillTextureName         ()
{
    return "HillTex";
}

// *******************************
//
std::string             DefaultHeightMapPluginDesc::CoveredDistTextureName  ()
{
    return "CoveredDistTex";
}

// *******************************
//
std::string             DefaultHeightMapPluginDesc::BackgroundTextureName   ()
{
    return "BackgroundTex";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultHeightMapPlugin::DefaultHeightMapPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_hmRawData( nullptr )
{
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultTexturePluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultTexturePluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );

    InitAttributesChannel( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    ctx->alphaCtx->blendEnabled = true;

    m_texturesData = m_psc->GetTexturesDataImpl();

    //Direct param state access (to bypass model querying)
    auto psModel = PixelShaderChannelModel();

    m_hmHeightScale     = QueryTypedParam< ParamFloatPtr >( GetParameter( "hmHeightScale" ) );
    m_GroundLevelHeight = QueryTypedParam< ParamFloatPtr >( GetParameter( "hmGroundLevelHeight" ) );
    m_MaxHeightValue    = QueryTypedParam< ParamFloatPtr >( GetParameter( "hmMaxHeightValue" ) );

}

// *************************************
// 
DefaultHeightMapPlugin::~DefaultHeightMapPlugin         ()
{
}

// *************************************
// 
bool                            DefaultHeightMapPlugin::LoadResource  ( IPluginResourceDescrConstPtr resDescr )
{
    //Order of texture uploads
    //1. heightmap
    //2. HillTexture
    //3. Covered Distance Texture
    //4. Background Texture
    std::string textureNames [] = { DefaultHeightMapPluginDesc::HeightMapTextureName(), 
                                    DefaultHeightMapPluginDesc::HillTextureName(),
                                    DefaultHeightMapPluginDesc::CoveredDistTextureName(),
                                    DefaultHeightMapPluginDesc::BackgroundTextureName() 
                                  };

    auto txData = m_psc->GetTexturesDataImpl();
    
    unsigned int curNumTextures = txData->GetTextures().size();

    assert( curNumTextures < 4 ); //FIXME: Second one may be added by a mask

    auto txResDescr = QueryTextureResourceDescr( resDescr );

    if ( txResDescr == nullptr )
    {
        return false;
    }

    //FIXME: use some better API to handle resources in general and textures in this specific case
    auto txDesc = DefaultTextureDescriptor::LoadTexture( txResDescr->GetTextureFile(), textureNames[ curNumTextures ] );

    if( txDesc != nullptr )
    {
        if( ( TextureSlot ) curNumTextures == TextureSlot::TS_HEIGHT_MAP )
        {
            m_hmRawData = ( const unsigned char * ) txDesc->GetBits()->Get();
        }

        SetTextureParams( ( TextureSlot ) curNumTextures, txDesc );

        txData->AddTexture( txDesc );

        return true;
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultHeightMapPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultHeightMapPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultHeightMapPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultHeightMapPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update();

    m_vaChannel->SetNeedsAttributesUpdate( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}

// *************************************
//
glm::vec2                           DefaultHeightMapPlugin::QueryPosition               ( float distInMeter ) const
{
    return glm::vec2( 0.0 );
}

// *************************************
//
void    DefaultHeightMapPlugin::InitAttributesChannel( IPluginPtr prev )
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

        auto verTexAttrChannel = new model::Float2AttributeChannel( desc, DefaultHeightMapPluginDesc::HeightMapTextureName(), true );

        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );
            verTexAttrChannel->AddAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
        }

        connComp->AddAttributeChannel( AttributeChannelPtr( verTexAttrChannel ) );

        m_vaChannel->AddConnectedComponent( connComp );
    }
}

// *************************************
//
void               DefaultHeightMapPlugin::SetTextureParams            ( TextureSlot slot, DefaultTextureDescriptor * txDesc ) const
{
    if( slot == TextureSlot::TS_HEIGHT_MAP )
    {
        assert( txDesc->GetHeight() == 1 );
    }

    txDesc->SetFilteringMode( TextureFilteringMode::TFM_POINT );
    txDesc->SetWrappingModeX( TextureWrappingMode::TWM_MIRROR );
    txDesc->SetWrappingModeY( TextureWrappingMode::TWM_MIRROR );
    txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
}

// *****************************
//
float             DefaultHeightMapPlugin::DecodeFixedPoint              ( const unsigned char * data ) const
{
    return float( ( ( data[ 2 ] << 8 ) | data[ 1 ] ) ) * 0.0625f;
}

// *****************************
//
float               DefaultHeightMapPlugin::DecodeHeight                ( const unsigned char * data, float scl, float groundLevel, float maxHeight ) const
{
    return scl * ( DecodeFixedPoint( data ) - groundLevel ) / ( maxHeight - groundLevel );
}

//// *****************************
//// FIXME: constant
//float decodeHeight( vec4 col )
//{
//	return hmHeightScale * ( decodeFixedPointValue( col.r, col.g, 1.0 / 16.0 ) - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight );
//}

} // model
} // bv
