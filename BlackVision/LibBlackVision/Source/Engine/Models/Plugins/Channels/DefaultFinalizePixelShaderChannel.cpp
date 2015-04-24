#include "DefaultFinalizePixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
//StaticShaderGenerator DefaultFinalizePixelShaderChannel::ms_staticShaderGenerator;

// *********************************
//
void     DefaultFinalizePixelShaderChannel::InitializePixelShaderMapping() const
{
    //m_staticShaderGenerator.InitializePixelShaderMapping();
}

// *********************************
//
DefaultFinalizePixelShaderChannel::DefaultFinalizePixelShaderChannel    ( IPixelShaderChannelPtr channel, const std::string & shadersDir )
    : Parent( channel, shadersDir )
    , m_staticShaderGenerator( shadersDir, "frag" )
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
    return m_staticShaderGenerator.GenerateShaderSource( uids );
}

} //model
} //bv

