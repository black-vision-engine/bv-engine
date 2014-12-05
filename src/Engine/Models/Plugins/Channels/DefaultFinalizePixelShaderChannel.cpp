#include "DefaultFinalizePixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
PluginUIDHashMap    DefaultFinalizePixelShaderChannel::ms_pixelShaderMapping;

// *********************************
//
void     DefaultFinalizePixelShaderChannel::InitializePixelShaderMapping() const
{
    if( ms_pixelShaderMapping.size() == 0 )
    {
        auto uidLists   = GetAcceptedPluginLists();
        auto baseNames  = GetBaseShaderFileNames();

        assert( uidLists.size() == baseNames.size() );

        for( size_t i = 0; i < uidLists.size(); ++i )
        {
            ms_pixelShaderMapping[ uidLists[ i ] ] = ShaderStorageDirectory() + baseNames[ i ] + ".frag";
        }
    }
}

// *********************************
//
DefaultFinalizePixelShaderChannel::DefaultFinalizePixelShaderChannel    ( IPixelShaderChannelPtr channel, const std::string & shadersDir )
    : Parent( channel, shadersDir )
{
    assert( channel != nullptr );

    InitializePixelShaderMapping();
} 

// *********************************
//
DefaultFinalizePixelShaderChannel::~DefaultFinalizePixelShaderChannel   ()
{
}

// *********************************
//
RendererContextConstPtr  DefaultFinalizePixelShaderChannel::GetRendererContext  () const
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
		printf( "Loading pixel shader from: %s\n", it->second.c_str() );
        return ReadShaderContentsFromFile( it->second );
    }

    assert( false );

    return "";
}

} //model
} //bv
