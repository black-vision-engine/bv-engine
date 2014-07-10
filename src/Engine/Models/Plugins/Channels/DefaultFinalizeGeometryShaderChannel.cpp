#include "DefaultFinalizeGeometryShaderChannel.h"


namespace bv { namespace model {


// *********************************
//
DefaultFinalizeGeometryShaderChannel::DefaultFinalizeGeometryShaderChannel  ( IGeometryShaderChannelPtr channel )
    : Parent( channel )
    , m_shaderSource( "" )
{
}

// *********************************
//
DefaultFinalizeGeometryShaderChannel::~DefaultFinalizeGeometryShaderChannel ()
{
}


} //model
} //bv
