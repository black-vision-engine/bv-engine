#include "SimpleTextPlugin.h"

#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/TextureLoader.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Resources/Font/FontLoader.h"
#include "Engine/Models/Resources/Font/Text.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "Engine/Events/Events.h"


namespace bv { namespace model {

const std::string SimpleTextPluginPD::pluginName = "SimpleTextPlugin";

// *********************************
//
SimpleTextPlugin* SimpleTextPlugin::Create( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold, bool italic )
{
    return new SimpleTextPlugin( text, fontFileName, fontSize, bold, italic );
}

// *********************************
//
SimpleTextPlugin::SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold, bool italic )
    : BasePlugin( nullptr )
    , m_text( text )
    , m_bolded( bold )
    , m_italic( italic )
    , m_atlasText()
    , m_textSet( true )
{

    auto res = LoadFont( fontFileName, fontSize );

    m_fontExtraData = static_cast< const FontExtraData* >( res->GetExtra() );

    m_textures.resize( 1 );

    LoadAtlas( "AtlasTex" ) ;

    EvalGeometryChannel();

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &SimpleTextPlugin::OnSetText ), SetTextEvent::Type() );

}

namespace
{


}

// *********************************
//
void SimpleTextPlugin::OnSetText                   ( IEventPtr evt )
{
    if( evt->GetEventType() == SetTextEvent::m_sEventType)
    {
        SetTextEventPtr evtTyped = std::static_pointer_cast<SetTextEvent>( evt );
        wchar_t c[2] = {evtTyped->GetChar() , '\0'};
        SetText( m_text + std::wstring( c ) );
    }
}

// *********************************
//
SimpleTextPlugin::~SimpleTextPlugin   ()
{
}

// *********************************
//
void SimpleTextPlugin::LoadAtlas( const std::string& name )
{
    delete m_atlasText;
    m_atlasText = nullptr;

    auto f = GetFont();

    if( f )
        m_atlasText = f->GetAtlas();

    if( !m_atlasText )
    {
        assert(!"Cannot load atlas");
    }

    unsigned int texSize = m_atlasText->GetWidth() * m_atlasText->GetHeight() * 4; //FIXME: Add format to atlas

    TextureExtraData* atlasExtraData = new TextureExtraData( m_atlasText->GetWidth(), m_atlasText->GetHeight(), 32, TextureFormat::F_A8R8G8B8, TextureType::T_2D );
    ResourceHandle* altasHandle = new ResourceHandle( const_cast< char* >(m_atlasText->GetData()), texSize, atlasExtraData );

    if( m_textures[ 0 ] )
    {
        delete m_textures[ 0 ];
    }

    m_textures[ 0 ] = new TextureInfo( altasHandle, name );
}


// *********************************
//
const Text *        SimpleTextPlugin::GetFont() const
{
    if( !m_bolded && !m_italic )
    { 
        assert( m_fontExtraData->GetFont() );
        return m_fontExtraData->GetFont();
    }

    if( m_bolded && m_italic )
    {
        assert( m_fontExtraData->GetFontBoldItalic() );
        return m_fontExtraData->GetFontBoldItalic();
    }

    if( m_italic )
    {
        assert( m_fontExtraData->GetFontItalic() );
        return m_fontExtraData->GetFontItalic();
    }

    if( m_bolded )
    {
        assert( m_fontExtraData->GetFontBold() );
        return m_fontExtraData->GetFontBold();
    }

    assert( false );

    return nullptr;
}

#define viewWidth   100
#define viewHeight  100

// *********************************
//
void                SimpleTextPlugin::EvalGeometryChannel( )
{
    auto texExtraData = static_cast< const TextureExtraData* > ( m_textures[ 0 ]->m_resHandle->GetExtra() );

    VertexAttributesChannelDescriptor geomChannelDesc;

    geomChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    geomChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    m_geomChannel = new VertexAttributesChannel( PrimitiveType::PT_TRIANGLE_STRIP, geomChannelDesc);

    m_geomChannel->SetNeedsTopologyUpdate( true ); //FIXME: TAK NIE WOLNO ROBIC, BO TO POWODUJE ODTWARZANIE GEMOETRII CO RAMKE, CHOCIAZ WCALE NIE TRZEBA

    //FIXME: zmieniam na false - Vig
    m_geomChannel->SetNeedsTopologyUpdate( false );

    glm::vec3 translate(0.f);
    glm::vec3 interspace( 0.07f, 0.f ,0.f );
    glm::vec3 newLineTranslation( 0.f );

    auto glyphH = m_atlasText->GetGlyphHeight();
    auto glyphW = m_atlasText->GetGlyphWidth();

    auto f = GetFont();

    for( auto wch : m_text )
    {
        if( wch == L' ' )
        {
            translate += glm::vec3( 0.3*(float)glyphW * 0.5/viewWidth, 0.f, 0.f )+ interspace;
            continue;
        }

        if( wch == L'\n' )
        {
            translate = glm::vec3( 0.f );
            newLineTranslation += glm::vec3( 0.f, (float)glyphH /viewHeight, 0.f );
            continue;
        }

        ConnectedComponent* connComp = new ConnectedComponent();

        AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        auto posAttribChannel = new Float3AttributeChannel( desc, "vertexPosition", true );

        auto glyph = f->GetGlyph( wch );

        glm::vec3 baring = glm::vec3( 0.f, (glyph->height - glyph->bearingY) / (float)viewHeight, 0.f );

        auto quadBottomLeft     = glm::vec3( 0.f, 0.f, 0.f );
        auto quadBottomRight    = glm::vec3( (float)glyph->width / (float)viewWidth, 0.f, 0.f );
        auto quadTopLeft        = glm::vec3( 0.f, (float)glyph->height / (float)viewHeight, 0.f );
        auto quadTopRight       = glm::vec3( (float)glyph->width / (float)viewWidth, (float)glyph->height / (float)viewHeight, 0.f );

        posAttribChannel->AddAttribute( quadBottomLeft    + translate - baring + newLineTranslation );
        posAttribChannel->AddAttribute( quadBottomRight   + translate - baring + newLineTranslation );
        posAttribChannel->AddAttribute( quadTopLeft       + translate - baring + newLineTranslation );
        posAttribChannel->AddAttribute( quadTopRight      + translate - baring + newLineTranslation );

        connComp->m_attributeChannels.push_back( posAttribChannel );

        AttributeChannelDescriptor * desc1 = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

        auto verTex0AttrChannel = new model::Float2AttributeChannel( desc1, m_textures[ 0 ]->m_texName, true );

        float texLeft   = ((float)glyph->textureX) / texExtraData->GetWidth();
        float texTop    = ((float)glyph->textureY) / texExtraData->GetHeight();
        float texWidth  = ((float)glyph->width) / texExtraData->GetWidth();
        float texHeight = ((float)glyph->height) / texExtraData->GetHeight();


        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop + texHeight ) );
        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop + texHeight ) );
        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft, texTop) );
        verTex0AttrChannel->AddAttribute( glm::vec2( texLeft + texWidth, texTop ) );

        connComp->m_attributeChannels.push_back( verTex0AttrChannel );

        m_geomChannel->AddConnectedComponent( connComp );

        translate += glm::vec3( glyph->width / (float)viewWidth, 0.f, 0.f ) + interspace;
    } 
}

// *********************************
//
const IVertexAttributesChannel *    SimpleTextPlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

// *********************************
//
void                SimpleTextPlugin::Update                      ( TimeType t )
{
    //FIXME: to jest nieco syf, ale nie taki, jak byl
    m_geomChannel->SetNeedsTopologyUpdate( m_textSet );
    m_geomChannel->Update( t );

    m_textSet = false;
}

// *********************************
//
void                SimpleTextPlugin::Print                       ( std::ostream & out, int tabs ) const
{
}

// *************************************
//
Textures            SimpleTextPlugin::GetTextures                 () const
{
    std::vector< TextureInfo* > prevTextures;
    if( m_prevPlugin )
    {
        prevTextures = m_prevPlugin->GetTextures();
    }
    prevTextures.insert( prevTextures.end(), m_textures.begin(), m_textures.end() );
    return prevTextures;
}


// *************************************
//
const EventType SetTextEvent::m_sEventType    = 0x12345674; //FIXME: nie pisac na pale tych idkow, bo sie rozjada z innymi - dlatego powinny byc generowane zbiorczo

// *************************************
//
std::string SetTextEvent::m_sEventName        = "Event_SetText";

// *************************************
//
void                SimpleTextPlugin::SetText                     ( const std::wstring & newText )
{
    m_textSet = true;
    m_text = newText;
}

// *************************************
//
SetTextEvent::SetTextEvent        ()
{}

// *************************************
//
EventType               SetTextEvent::GetEventType      () const
{
    return m_sEventType;
}

// *************************************
//
IEventPtr               SetTextEvent::Clone             () const
{
    return IEventPtr( new SetTextEvent( *this ) );
}

// *************************************
//
const std::string &     SetTextEvent::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
void                    SetTextEvent::SetChar             ( unsigned char  c )
{
    m_char = c;
}

// *************************************
//
unsigned char           SetTextEvent::GetChar             () const
{
    return m_char;
}

// *************************************
//
EventType               SetTextEvent::Type                ()
{
    return m_sEventType;
}

} // model
} // bv
