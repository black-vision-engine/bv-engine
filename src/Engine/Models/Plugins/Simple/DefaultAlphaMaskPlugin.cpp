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

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultAlphaMaskPluginDesc::DefaultAlphaMaskPluginDesc                      ()
    : BasePluginDescriptor( UID(), "alpha_mask" )
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

    if ( uid != DefaultColorPluginDesc::UID() && uid != DefaultTexturePluginDesc::UID() && uid != DefaultTextPluginDesc::UID() && uid != DefaultAnimationPluginDesc::UID() )
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
    return "../dep/media/shaders/dummy.vert"; //FIXME: deprecated
}

// *******************************
//
std::string             DefaultAlphaMaskPluginDesc::PixelShaderSource       ()
{
    return "../dep/media/shaders/dummy.frag"; //FIXME: deprecated
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
bool                        DefaultAlphaMaskPlugin::LoadResource  ( IPluginResourceDescrConstPtr resDescr )
{
    auto txResDescr = QueryTextureResourceDescr( resDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txResDescr->GetTextureFile(), DefaultAlphaMaskPluginDesc::TextureName() );

        //Alpha texture defaults
        txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
        txDesc->SetBorderColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
        txDesc->SetWrappingModeX( TextureWrappingMode::TWM_CLAMP_BORDER );
        txDesc->SetWrappingModeY( TextureWrappingMode::TWM_CLAMP_BORDER );
        txDesc->SetFilteringMode( TextureFilteringMode::TFM_LINEAR );

        if( txDesc != nullptr )
        {
            //FIXME: albo slid, wtedy podmien - to mozna tu jeszcze ogarnac
            //FIXME: albo tekstura, wtedy podmien ostatnia lub dodaj - to mozna tu jeszcze ogarnac
            //FIXME: albo animacja, wtedy podmien - to mozna tu jeszcze ogarnac
            if( txData->GetTextures().size() == 0 || txData->GetTextures().size() == 1 )
            {
                txData->AddTexture( txDesc );
            }
            else
            {
                txData->SetTexture( txData->GetTextures().size(), txDesc );
            }

            m_textureWidth = txDesc->GetWidth();
            m_textureHeight = txDesc->GetHeight();

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
    m_paramValModel->Update();

    auto attachmentMode = GetAttachementMode();

    if( m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() ) //FIXME: additionalna hackierka
    {
        m_vaChannel = nullptr;

        assert( m_prevPlugin->GetTypeUid() == DefaultTextPluginDesc::UID() );

        InitAttributesChannel( m_prevPlugin );

        //FIXME: very fucking hack - treats text connected components as FREE uv mapping
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

    if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() || StateChanged( wX, wY, fm, attachmentMode ) )
    {
        UpdateState( wX, wY, fm, attachmentMode );
        m_vaChannel->SetNeedsAttributesUpdate( true );
    }
    else
    {
        m_vaChannel->SetNeedsAttributesUpdate( false );
    }

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void DefaultAlphaMaskPlugin::InitAttributesChannel( IPluginPtr prev )
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

        auto verTexAttrChannel = new model::Float2AttributeChannel( desc, DefaultAlphaMaskPluginDesc::TextureName(), true );

        for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
        {
            const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( prevCompChannels[ 0 ]->GetData() );
            verTexAttrChannel->AddAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
        }

        connComp->AddAttributeChannel( AttributeChannelPtr( verTexAttrChannel ) );

        m_vaChannel->AddConnectedComponent( connComp );
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
int                                         DefaultAlphaMaskPlugin::GetAlphaTextureWidth    () const
{
    return m_textureWidth;
}

// *************************************
//
int                                         DefaultAlphaMaskPlugin::GetAlphaTextureHeight   () const
{
    return m_textureHeight;
}

} // model
} // bv
