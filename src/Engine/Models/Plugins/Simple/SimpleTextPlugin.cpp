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
#include "Engine/Models/Resources/Font/TextHelper.h"
#include "Engine/Events/Interfaces/IEventManager.h"


#include "Engine/Events/Events.h"


namespace bv { namespace model {


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
    , m_fontResource()
    , m_textSet( true )
{
    m_fontResource = TextHelper::LoadFont( fontFileName, fontSize );

    m_textures.push_back( TextHelper::GetAtlasTextureInfo( m_fontResource, "AtlasTex" ) );

    m_vertexAttributeChannel = VertexAttributesChannelPtr( TextHelper::CreateVACForText() );

    TextHelper::BuildVACForText( m_vertexAttributeChannel.get(), m_fontResource, m_text );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &SimpleTextPlugin::OnSetText ), KeyPressedEvent::Type() );

}

// *********************************
//
void SimpleTextPlugin::OnSetText                   ( IEventPtr evt )
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

// *********************************
//
SimpleTextPlugin::~SimpleTextPlugin   ()
{
}

// *********************************
//
const IVertexAttributesChannel *    SimpleTextPlugin::GetGeometryChannel          () const
{
    return m_vertexAttributeChannel.get();
}

// *********************************
//
void                SimpleTextPlugin::Update                      ( TimeType t )
{
    //FIXME: to jest nieco syf, ale nie taki, jak byl
    m_vertexAttributeChannel->SetNeedsTopologyUpdate( m_textSet );
    m_vertexAttributeChannel->Update( t );

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
void                SimpleTextPlugin::SetText                     ( const std::wstring & newText )
{
    m_textSet = true;
    m_text = newText;

    m_vertexAttributeChannel->ClearConnectedComponent();

    TextHelper::BuildVACForText( m_vertexAttributeChannel.get(), m_fontResource, m_text );
}

} // model
} // bv
