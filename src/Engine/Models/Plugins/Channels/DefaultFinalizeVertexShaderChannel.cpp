#include "DefaultFinalizeVertexShaderChannel.h"


namespace bv { namespace model {


// *********************************
//
DefaultFinalizeVertexShaderChannel::DefaultFinalizeVertexShaderChannel  ( IVertexShaderChannelPtr channel )
    : Parent( channel )
    , m_shaderSource( "" )
{
}

// *********************************
//
DefaultFinalizeVertexShaderChannel::~DefaultFinalizeVertexShaderChannel ()
{
}


} //model
} //bv
