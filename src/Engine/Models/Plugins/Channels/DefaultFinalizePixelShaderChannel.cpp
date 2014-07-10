#include "DefaultFinalizePixelShaderChannel.h"


namespace bv { namespace model {

// *********************************
PluginUIDHashMap    DefaultFinalizePixelShaderChannel::ms_pixelShaderMapping;

// *********************************
//
void     DefaultFinalizePixelShaderChannel::InitializePixelShaderMapping()
{
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
        return it->second;
    }

    assert( false );

    return "";
}

} //model
} //bv
