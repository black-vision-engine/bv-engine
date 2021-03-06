#include "stdafx.h"

#include "DefaultFinalizeGeometryShaderChannel.h"




#include "Memory/MemoryLeaks.h"



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
    { uids; } // FIXME: suppress unused variable

    assert( false );

    return "";
}

} //model
} //bv
