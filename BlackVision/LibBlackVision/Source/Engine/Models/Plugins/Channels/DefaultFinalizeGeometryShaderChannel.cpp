#include "DefaultFinalizeGeometryShaderChannel.h"


namespace bv { namespace model {


// *********************************
//
DefaultFinalizeGeometryShaderChannel::DefaultFinalizeGeometryShaderChannel      ( IGeometryShaderChannelPtr channel, const std::string & shadersDir )
    : Parent( channel, shadersDir )
{
}

// *********************************
//
DefaultFinalizeGeometryShaderChannel::~DefaultFinalizeGeometryShaderChannel     ()
{
}

// *********************************
//
std::string DefaultFinalizeGeometryShaderChannel::GetShaderSource               ( const std::vector< std::string > & uids )  const
{
    assert( false );

    return "";
}

} //model
} //bv
