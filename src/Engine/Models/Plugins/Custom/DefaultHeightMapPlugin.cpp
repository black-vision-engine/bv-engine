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
    SimpleVec4EvaluatorPtr      borderColorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

    SimpleFloatEvaluatorPtr     offsetXEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "offsetX", timeEvaluator );
    SimpleFloatEvaluatorPtr     scaleXEvaluator         = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "scaleX", timeEvaluator );
    SimpleFloatEvaluatorPtr     coveredDistEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "coveredDist", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    vsModel->RegisterAll( offsetXEvaluator );
    vsModel->RegisterAll( scaleXEvaluator );
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    psModel->RegisterAll( coveredDistEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

    //FIXME: integer parmeters should be used here
    offsetXEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    scaleXEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    coveredDistEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );

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

    txDesc->SetFilteringMode( TextureFilteringMode::TFM_LINEAR );
    txDesc->SetWrappingModeX( TextureWrappingMode::TWM_MIRROR );
    txDesc->SetWrappingModeY( TextureWrappingMode::TWM_MIRROR );
    txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
}

} // model
} // bv
