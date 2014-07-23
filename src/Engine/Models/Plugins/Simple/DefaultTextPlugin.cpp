#include "DefaultTextPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Resources/IPluginResourceDescr.h"

#include "Engine/Models/Resources/Font/FontLoader.h"
#include "Engine/Models/Resources/Font/Text.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTextPluginDesc::DefaultTextPluginDesc                            ()
    : BasePluginDescriptor( UID(), "text" )
{
}

// *******************************
//
IPluginPtr              DefaultTextPluginDesc::CreatePlugin             ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTextPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTextPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr plModel      = std::make_shared< DefaultParamValModel >();


    //Create all parameters and evaluators
    SimpleVec4EvaluatorPtr      borderColorEvaluator    = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator          = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator           = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    SimpleFloatEvaluatorPtr     fontSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "fontSize", timeEvaluator );

    SimpleFloatEvaluatorPtr     blurSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );

    SimpleFloatEvaluatorPtr     spacingEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "spacing", timeEvaluator );
    SimpleFloatEvaluatorPtr     alignmentEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alignment", timeEvaluator );
    SimpleFloatEvaluatorPtr     maxTextLenghtEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "maxTextLenght", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    plModel->RegisterAll( blurSizeEvaluator );
    plModel->RegisterAll( spacingEvaluator );
    plModel->RegisterAll( alignmentEvaluator );
    plModel->RegisterAll( fontSizeEvaluator );
    plModel->RegisterAll( maxTextLenghtEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );
    model->SetPluginModel( plModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    blurSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    spacingEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    alignmentEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );
    maxTextLenghtEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultTextPluginDesc::CanBeAttachedTo   ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return true;
    }

    auto  vac = plugin->GetVertexAttributesChannel();
    if ( vac != nullptr )
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string             DefaultTextPluginDesc::UID                      ()
{
    return "DEFAULT_TEXT";
}

// *******************************
//
std::string             DefaultTextPluginDesc::VertexShaderSource       ()
{
    return "../dep/media/shaders/dummy.vert";   //FIXME: deprecated
}

// *******************************
//
std::string             DefaultTextPluginDesc::PixelShaderSource        ()
{
    return "../dep/media/shaders/dummy.frag";   //FIXME: deprecated
}

// *******************************
//
std::string             DefaultTextPluginDesc::TextureName              ()
{
    return "AtlasTex0";
}

// *******************************
//
std::string             DefaultTextPluginDesc::FontFileName             ()
{
    return "../dep/Media/fonts/ARIALUNI.TTF";
}

// *************************************
// 
DefaultTextPlugin::DefaultTextPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_textSet( true )
    , m_textAtlas( nullptr )
    , m_text( L"" )
{
    auto colorParam = prev->GetParameter( "color" );

    if ( colorParam == nullptr )
    {
        auto bcParam = this->GetParameter( "borderColor" );
        SimpleVec4EvaluatorPtr      colorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", bcParam->GetTimeEvaluator() );
        std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( colorEvaluator );
        colorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 1.f, 1.f, 1.f ), TimeType( 0.f ) );
    }
    else
    {
        auto evaluators = prev->GetPluginParamValModel()->GetPixelShaderChannelModel()->GetEvaluators();
        for( unsigned int i = 0; i < evaluators.size(); ++i )
        {
            auto colorParam = evaluators[ i ]->GetParameter( "color" );
            if( colorParam != nullptr )
            {
                //FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak)
                std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( evaluators[ i ] );
                break;
            }
        }
        
    }


    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultTextPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultTextPluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    ctx->alphaCtx->blendEnabled = true;

    ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
    ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;

    m_texturesData = m_psc->GetTexturesDataImpl();

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &DefaultTextPlugin::OnSetText ), KeyPressedEvent::Type() );

    m_fontSizeParam         = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "fontSize" ) );
    m_blurSizeParam         = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "blurSize" ) );
    m_spacingParam          = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );
    m_alignmentParam        = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "alignment" ) );
    m_maxTextLengthParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "maxTextLenght" ) );
}

// *************************************
// 
DefaultTextPlugin::~DefaultTextPlugin         ()
{
}

// *************************************
// 
bool                            DefaultTextPlugin::LoadResource  ( IPluginResourceDescrConstPtr resDescr )
{
    auto txResDescr = QueryFontResourceDescr( resDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 );

        auto fontResource = TextHelper::LoadFont( txResDescr->GetFontFile(), int( m_fontSizeParam->Evaluate() ), int( m_blurSizeParam->Evaluate() ) );

        m_textAtlas = TextHelper::GetAtlas( fontResource.get(), false, false );

        auto textureResource = TextHelper::GetAtlasTextureInfo( m_textAtlas );

        auto txInfo = new TextureInfo( textureResource.get(), DefaultTextPluginDesc::TextureName() );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = new DefaultTextureDescriptor(     textureResource
                                                    ,   DefaultTextPluginDesc::TextureName()
                                                    ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                                    ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                                    ,   TextureFilteringMode::TFM_LINEAR
                                                    ,   glm::vec4( 0.f, 0.f, 0.f, 0.f )
                                                    ,   DataBuffer::Semantic::S_TEXTURE_STATIC );

        if( txDesc != nullptr )
        {
            if( txData->GetTextures().size() == 0 )
            {
                txData->AddTexture( txDesc );
            }
            else
            {
                txData->SetTexture( 0, txDesc );
            }

            InitAttributesChannel( m_prevPlugin );

            return true;
        }
    }    

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultTextPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultTextPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultTextPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultTextPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update();

    m_vaChannel->SetNeedsTopologyUpdate( m_textSet );

    m_textSet = false;

    //if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
    //{
    //    m_vaChannel->SetNeedsAttributesUpdate( true );
    //}
    //else
    //{
    //    m_vaChannel->SetNeedsAttributesUpdate( false );
    //}

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

namespace {
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
void DefaultTextPlugin::InitAttributesChannel( IPluginPtr prev )
{
    m_vaChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );

    auto alignType =  EvaluateAsInt< TextAlignmentType >( m_alignmentParam );

    TextHelper::BuildVACForText( m_vaChannel.get(), m_textAtlas, m_text, unsigned int( m_blurSizeParam->Evaluate() ), m_spacingParam->Evaluate(), alignType );
}

// *************************************
//
void DefaultTextPlugin::OnSetText                   ( IEventPtr evt )
{
    if( evt->GetEventType() == KeyPressedEvent::m_sEventType)
    {
        KeyPressedEventPtr evtTyped = std::static_pointer_cast<KeyPressedEvent>( evt );
        wchar_t c[2] = {evtTyped->GetChar() , '\0'};

        if( c[0] == L'\b' )
        {
            if( !m_text.empty() )
            {
                m_text.pop_back();
                SetText( m_text );
            }
        }
        else
            SetText( m_text + std::wstring( c ) );
    }
}

//namespace 
//{
//// *************************************
////
//glm::vec2 GetLeftAndRightCorner( const VertexAttributesChannelPtr & vaChannel )
//{
//    auto components = vaChannel->GetComponents();
//
//    glm::vec2 ret;
//
//    if( components.size() > 0 )
//    {
//        auto attrChannels = ret.x = components[ 0 ]->GetAttributeChannels();
//
//        if( attrChannels.size() > 0 )
//            ret.x = attrChannels[ 0 ].
//
//        [ 0 ].x;
//        ret.y = components[ 0 ].y;
//    }
//
//
//    for components
//}
//
//} // anonymous

// *************************************
//
void DefaultTextPlugin::SetText                     ( const std::wstring & newText )
{
    m_textSet = true;
    m_text = newText;

    m_vaChannel->ClearConnectedComponent();

    auto alignType =  EvaluateAsInt< TextAlignmentType >( m_alignmentParam );

    auto textLength = TextHelper::BuildVACForText( m_vaChannel.get(), m_textAtlas, m_text, unsigned int( m_blurSizeParam->Evaluate() ), m_spacingParam->Evaluate(), alignType );

    auto maxTextLenght = m_maxTextLengthParam->Evaluate();

    if( maxTextLenght > 0.f && textLength > 0.f && textLength > maxTextLenght )
    {
        auto transformChannel = m_prevPlugin->GetTransformChannel();

        if( transformChannel != nullptr )
        {
            auto& transformValues = transformChannel->GetTransformValues();

            auto currTransform = transformValues[ 0 ]->GetValue(); // FIXME: Czujesz jak bardzo to ssie?

            // FIXME: A to tak ssie, ze trzeba przescieradlo z dupy wyciagac.
            const_cast< ValueMat4PtrVec & >( transformValues )[ 0 ]->SetValue( glm::scale( currTransform, glm::vec3( maxTextLenght / textLength, 1.f, 1.f ) ) );  
        }
    }

    m_vaChannel->SetNeedsTopologyUpdate( true );
}

// *************************************
//
bool            SetTextPluginContent( IPluginPtr textPlugin, const std::wstring& text )
{
    if( textPlugin->GetTypeUid() == DefaultTextPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTextPlugin >( textPlugin )->SetText( text );
        return true;
    }
    else
        return false;
}

} // model
} // bv
