#include "stdafx.h"

#include "DefaultFinalizeVertexShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
//
DefaultFinalizeVertexShaderChannel::DefaultFinalizeVertexShaderChannel      ( IVertexShaderChannelPtr channel, const std::string & shadersDir )
    : Parent( channel, shadersDir )
    , m_staticShaderGenerator( shadersDir, "vert" )
{
    assert( channel != nullptr );
}

// *********************************
//
DefaultFinalizeVertexShaderChannel::~DefaultFinalizeVertexShaderChannel     ()
{
}

// *********************************
//
std::string     DefaultFinalizeVertexShaderChannel::GetShaderSource         ( const std::vector< std::string > & uids ) const
{
    return m_staticShaderGenerator.GenerateShaderSource( uids );
}

} //model
} //bv
