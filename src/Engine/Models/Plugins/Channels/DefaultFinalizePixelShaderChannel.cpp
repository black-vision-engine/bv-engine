#include "DefaultFinalizePixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
PluginUIDHashMap    DefaultFinalizePixelShaderChannel::ms_pixelShaderMapping;

// *********************************
//
void     DefaultFinalizePixelShaderChannel::InitializePixelShaderMapping()
{
    auto uidLists   = GetAcceptedPluginLists();
    auto baseNames  = GetBaseShaderFileNames();

    assert( uidLists.size() == baseNames.size() );

    for( size_t i = 0; i < uidLists.size(); ++i )
    {
        ms_pixelShaderMapping[ uidLists[ i ] ] = baseNames[ i ] + ".frag";
    }
}

// *********************************
//
DefaultFinalizePixelShaderChannel::DefaultFinalizePixelShaderChannel    ( IPixelShaderChannelPtr channel )
    : Parent( channel )
{
} 

// *********************************
//
DefaultFinalizePixelShaderChannel::~DefaultFinalizePixelShaderChannel   ()
{
}

// *********************************
//
RendererContextConstPtr     DefaultFinalizePixelShaderChannel::GetRendererContext  () const
{
    return m_channel->GetRendererContext();
}

// *********************************
//
std::string     DefaultFinalizePixelShaderChannel::GetShaderSource  ( const std::vector< std::string > & uids ) const
{
    auto it = ms_pixelShaderMapping.find( uids );

    if( it != ms_pixelShaderMapping.end() )
    {
        return ReadShaderContentsFromFile( it->second );
    }

    assert( false );

    return "";
}

} //model
} //bv
