#include "DefaultFinalizeVertexShaderChannel.h"


namespace bv { namespace model {

// *********************************
PluginUIDHashMap    DefaultFinalizeVertexShaderChannel::ms_vertexShaderMapping;

// *********************************
//
void     DefaultFinalizeVertexShaderChannel::InitializeVertexShaderMapping  ()
{
    
}

// *********************************
//
DefaultFinalizeVertexShaderChannel::DefaultFinalizeVertexShaderChannel      ( IVertexShaderChannelPtr channel )
    : Parent( channel )
{
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
    auto it = ms_vertexShaderMapping.find( uids );

    if( it != ms_vertexShaderMapping.end() )
    {
        return it->second;
    }

    assert( false );

    return "";
}

} //model
} //bv
