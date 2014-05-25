#include "DefaultTextPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Resources/IPluginResourceDescr.h"

#include "Engine/Models/Resources/Font/TextHelper.h"
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
IPlugin *               DefaultTextPluginDesc::CreatePlugin             ( const std::string & name, const IPlugin * prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTextPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModel *    DefaultTextPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModel * model  = new DefaultPluginParamValModel();
    DefaultParamValModel * psModel      = new DefaultParamValModel();
    DefaultParamValModel * vsModel      = new DefaultParamValModel();
    DefaultParamValModel * plModel      = new DefaultParamValModel();


    //Create all parameters and evaluators
    SimpleVec4Evaluator *      borderColorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
    SimpleFloatEvaluator *     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluator * trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    SimpleFloatEvaluator *     fontSizeEvaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "fontSize", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    plModel->RegisterAll( fontSizeEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );
    model->SetPluginModel( plModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultTextPluginDesc::CanBeAttachedTo   ( const IPlugin * plugin ) const
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

    auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
    if ( numChannels != 1 ) //only vertex attribute data allowed here
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
    return "../dep/media/shaders/text.vert";
}

// *******************************
//
std::string             DefaultTextPluginDesc::PixelShaderSource        ()
{
    return "../dep/media/shaders/text.frag";
}

// *******************************
//
std::string             DefaultTextPluginDesc::TextureName              ()
{
    return "AtlasTex";
}

// *******************************
//
std::string             DefaultTextPluginDesc::FontFileName             ()
{
    return "../dep/Media/fonts/ARIALUNI.TTF";
}

// *************************************
// 
DefaultTextPlugin::DefaultTextPlugin         ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_textSet( true )
{
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultTextPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultTextPluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );

    InitAttributesChannel( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;

    m_texturesData = m_psc->GetTexturesDataImpl();

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &DefaultTextPlugin::OnSetText ), KeyPressedEvent::Type() );
}

// *************************************
// 
DefaultTextPlugin::~DefaultTextPlugin         ()
{
}

// *************************************
// 
bool                            DefaultTextPlugin::LoadResource  ( const IPluginResourceDescr * resDescr )
{
    auto txResDescr = QueryFontResourceDescr( resDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 );

        auto fontResource = TextHelper::LoadFont( txResDescr->GetFontFile(), 8 );

        m_textAtlas = TextHelper::GetAtlas( fontResource, false, false );

        auto textureResource = TextHelper::GetAtlasTextureInfo( m_textAtlas );

        auto txInfo = new TextureInfo( textureResource, DefaultTextPluginDesc::TextureName() );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = new DefaultTextureDescriptor(     m_textAtlas->GetData()
                                                    ,   DefaultTextPluginDesc::TextureName()
                                                    ,   m_textAtlas->GetWidth()
                                                    ,   m_textAtlas->GetHeight()
                                                    ,   TextureFormat::F_A8R8G8B8
                                                    ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                                    ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                                    ,   TextureFilteringMode::TFM_LINEAR
                                                    ,   glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

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

            return true;
        }
    }

    return false;
}

// *************************************
// 
const IVertexAttributesChannel *    DefaultTextPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel.get();
}

// *************************************
// 
const IPixelShaderChannel *         DefaultTextPlugin::GetPixelShaderChannel       () const
{
    return m_psc.get();
}

// *************************************
// 
const IVertexShaderChannel *        DefaultTextPlugin::GetVertexShaderChannel      () const
{
    return m_vsc.get();
}

// *************************************
// 
void                                DefaultTextPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update();

    m_vaChannel->SetNeedsTopologyUpdate( m_textSet );

    m_textSet = false;

    if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
    {
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
void DefaultTextPlugin::InitAttributesChannel( const IPlugin * prev )
{
    m_vaChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );

    TextHelper::BuildVACForText( m_vaChannel.get(), m_textAtlas, m_text );
}

// *************************************
//
void DefaultTextPlugin::OnSetText                   ( IEventPtr evt )
{
    if( evt->GetEventType() == KeyPressedEvent::m_sEventType)
    {
        KeyPressedEventPtr evtTyped = std::static_pointer_cast<KeyPressedEvent>( evt );
        wchar_t c[2] = {evtTyped->GetChar() , '\0'};

        if( c[0] == L'\b' && !m_text.empty() )
        {
            m_text.pop_back();
            SetText( m_text );
        }
        else
            SetText( m_text + std::wstring( c ) );
    }
}

// *************************************
//
void DefaultTextPlugin::SetText                     ( const std::wstring & newText )
{
    m_textSet = true;
    m_text = newText;

    m_vaChannel->ClearConnectedComponent();

    TextHelper::BuildVACForText( m_vaChannel.get(), m_textAtlas, m_text );
}

} // model
} // bv

//#include "SimpleTextPlugin.h"
//
//#include "Engine/Models/Resources/IResource.h"
//#include "Engine/Models/Resources/TextureLoader.h"
//
//#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
//#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
//#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
//#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
//#include "Engine/Models/Resources/Font/FontLoader.h"
//#include "Engine/Models/Resources/Font/Text.h"
//#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
//#include "Engine/Models/Resources/Font/TextHelper.h"
//#include "Engine/Events/Interfaces/IEventManager.h"
//
//
//#include "Engine/Events/Events.h"
//
//
//namespace bv { namespace model {
//
//
//// *********************************
////
//SimpleTextPlugin* SimpleTextPlugin::Create( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold, bool italic )
//{
//    return new SimpleTextPlugin( text, fontFileName, fontSize, bold, italic );
//}
//
//// *********************************
////
//SimpleTextPlugin::SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold, bool italic )
//    : BasePlugin( "dupa", "dupa", nullptr, nullptr )
//    , m_text( text )
//    , m_bolded( bold )
//    , m_italic( italic )
//    , m_textAtlas()
//    , m_fontResource()
//    , m_textSet( true )
//{
//    m_fontResource = TextHelper::LoadFont( fontFileName, fontSize );
//
//    m_textAtlas = TextHelper::GetAtlas( m_fontResource, m_bolded, m_italic );
//
//    auto textureResource = TextHelper::GetAtlasTextureInfo( m_textAtlas );
//
//    m_textures.push_back( new TextureInfo( textureResource, "AtlasTex" ) );
//
//    m_vertexAttributeChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );
//
//    TextHelper::BuildVACForText( m_vertexAttributeChannel.get(), m_textAtlas, m_text );
//
//    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &SimpleTextPlugin::OnSetText ), KeyPressedEvent::Type() );
//
//}
//
//// *********************************
////
//void SimpleTextPlugin::OnSetText                   ( IEventPtr evt )
//{
//    if( evt->GetEventType() == KeyPressedEvent::m_sEventType)
//    {
//        KeyPressedEventPtr evtTyped = std::static_pointer_cast<KeyPressedEvent>( evt );
//        wchar_t c[2] = {evtTyped->GetChar() , '\0'};
//
//        if( c[0] == L'\b' && !m_text.empty() )
//        {
//            m_text.pop_back();
//            SetText( m_text );
//        }
//        else
//            SetText( m_text + std::wstring( c ) );
//    }
//}
//
//// *********************************
////
//SimpleTextPlugin::~SimpleTextPlugin   ()
//{
//}
//
//// *********************************
////
//const IVertexAttributesChannel *    SimpleTextPlugin::GetVertexAttributesChannel          () const
//{
//    return m_vertexAttributeChannel.get();
//}
//
//// *********************************
////
//void                SimpleTextPlugin::Update                      ( TimeType t )
//{
//    //FIXME: to jest nieco syf, ale nie taki, jak byl
//    m_vertexAttributeChannel->SetNeedsTopologyUpdate( m_textSet );
//    m_vertexAttributeChannel->Update( t );
//
//    m_textSet = false;
//}
//
//// *********************************
////
//void                SimpleTextPlugin::Print                       ( std::ostream & out, int tabs ) const
//{
//}
//
//// *************************************
////
//TextureInfoVec      SimpleTextPlugin::GetTextures                 () const
//{
//    return TextureInfoVec();
//
//    //std::vector< TextureInfo * > prevTextures;
//
//    //if( m_prevPlugin )
//    //{
//    //    
//    //    prevTextures = m_prevPlugin->GetTextures();
//    //}
//
//    //prevTextures.insert( prevTextures.end(), m_textures.begin(), m_textures.end() );
//
//    //return prevTextures;
//}
//
//
//// *************************************
////
//void                SimpleTextPlugin::SetText                     ( const std::wstring & newText )
//{
//    m_textSet = true;
//    m_text = newText;
//
//    m_vertexAttributeChannel->ClearConnectedComponent();
//
//    TextHelper::BuildVACForText( m_vertexAttributeChannel.get(), m_textAtlas, m_text );
//}
//
//} // model
//} // bv

