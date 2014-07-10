#include "DefaultFinalizeVertexShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
PluginUIDHashMap    DefaultFinalizeVertexShaderChannel::ms_vertexShaderMapping;

// *********************************
//
void     DefaultFinalizeVertexShaderChannel::InitializeVertexShaderMapping  ()
{
    if( ms_vertexShaderMapping.size() == 0 )
    {
        auto uidLists   = GetAcceptedPluginLists();
        auto baseNames  = GetBaseShaderFileNames();

        assert( uidLists.size() == baseNames.size() );

        for( size_t i = 0; i < uidLists.size(); ++i )
        {
            ms_vertexShaderMapping[ uidLists[ i ] ] = ShaderStorageDirectory() + baseNames[ i ] + ".vert";
        }    
    }
}

// *********************************
//
DefaultFinalizeVertexShaderChannel::DefaultFinalizeVertexShaderChannel      ( IVertexShaderChannelPtr channel )
    : Parent( channel )
{
    assert( channel != nullptr );

    InitializeVertexShaderMapping();
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
        return ReadShaderContentsFromFile( it->second );
    }

    assert( false );

    return "";
}

} //model
} //bv
