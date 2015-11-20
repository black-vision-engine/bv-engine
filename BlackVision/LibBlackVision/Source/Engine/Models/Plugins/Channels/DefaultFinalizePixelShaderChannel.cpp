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
IPixelShaderChannelPtr	DefaultFinalizePixelShaderChannel::GetChannel			() const
{
	return m_channel;
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

// *********************************
//
UInt64			DefaultFinalizePixelShaderChannel::GetTexturesDataUpdateID		() const
{
	return m_channel->GetTexturesDataUpdateID();
}

// *********************************
//
UInt64			DefaultFinalizePixelShaderChannel::GetRendererContextUpdateID	() const
{
	return m_channel->GetRendererContextUpdateID();
}

} //model
} //bv

