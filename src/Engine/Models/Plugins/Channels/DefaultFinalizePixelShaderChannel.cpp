#include "DefaultFinalizePixelShaderChannel.h"


namespace bv { namespace model {


// *********************************
//
DefaultFinalizePixelShaderChannel::DefaultFinalizePixelShaderChannel  ( IPixelShaderChannelPtr channel )
    : Parent( channel )
    , m_shaderSource( "" )
{
}

// *********************************
//
DefaultFinalizePixelShaderChannel::~DefaultFinalizePixelShaderChannel ()
{
}


} //model
} //bv
