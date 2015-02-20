#include "DefaultFinalizePixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
PluginUIDHashMap    StaticShaderGenerator::ms_pixelShaderMapping;
//StaticShaderGenerator DefaultFinalizePixelShaderChannel::ms_staticShaderGenerator;

// *********************************
//
void     DefaultFinalizePixelShaderChannel::InitializePixelShaderMapping() const
{
    ms_staticShaderGenerator.InitializePixelShaderMapping();
}

// *********************************
//
DefaultFinalizePixelShaderChannel::DefaultFinalizePixelShaderChannel    ( IPixelShaderChannelPtr channel, const std::string & shadersDir )
    : Parent( channel, shadersDir )
    , ms_staticShaderGenerator( channel, shadersDir )
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
    return ms_staticShaderGenerator.GenerateShaderSource( uids );
}

} //model
} //bv

