#include "DefaultFinalizePixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
//
DefaultFinalizePixelShaderChannel::DefaultFinalizePixelShaderChannel    ( IPixelShaderChannelPtr channel, const std::string & shadersDir )
    : Parent( channel, shadersDir )
    , m_staticShaderGenerator( shadersDir, "frag" )
{
    assert( channel != nullptr );
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
    return m_staticShaderGenerator.GenerateShaderSource( uids );
}

} //model
} //bv

