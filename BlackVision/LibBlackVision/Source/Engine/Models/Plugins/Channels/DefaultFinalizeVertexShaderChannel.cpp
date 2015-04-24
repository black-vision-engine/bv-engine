#include "DefaultFinalizeVertexShaderChannel.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

// *********************************
//
void     DefaultFinalizeVertexShaderChannel::InitializeVertexShaderMapping  () const
{
    //if( ms_vertexShaderMapping.size() == 0 )
    //{
    //    auto uidLists   = GetAcceptedPluginLists();
    //    auto baseNames  = GetBaseShaderFileNames();

    //    assert( uidLists.size() == baseNames.size() );

    //    for( size_t i = 0; i < uidLists.size(); ++i )
    //    {
    //        ms_vertexShaderMapping[ uidLists[ i ] ] = ShaderStorageDirectory() + baseNames[ i ] + ".vert";
    //    }    
    //}
}

// *********************************
//
DefaultFinalizeVertexShaderChannel::DefaultFinalizeVertexShaderChannel      ( IVertexShaderChannelPtr channel, const std::string & shadersDir )
    : Parent( channel, shadersDir )
    , m_staticShaderGenerator( shadersDir, "vert" )
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
    return m_staticShaderGenerator.GenerateShaderSource( uids );
}

} //model
} //bv
