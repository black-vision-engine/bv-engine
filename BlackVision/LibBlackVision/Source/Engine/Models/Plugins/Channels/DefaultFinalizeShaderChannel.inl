#include <vector>

#include "StaticShaderGenerator.h"

namespace bv { namespace model {

//namespace {

// *********************************
template< typename ShaderChannelIface >
std::vector< std::vector< std::string > >   DefaultFinalizeShaderChannel< ShaderChannelIface >::ms_acceptedPluginLists;

// *********************************
template< typename ShaderChannelIface >
std::vector< std::string >                  DefaultFinalizeShaderChannel< ShaderChannelIface >::ms_baseShaderFileNames;


// *********************************
//
template< typename ShaderChannelIface >
DefaultFinalizeShaderChannel< ShaderChannelIface >::DefaultFinalizeShaderChannel    ( std::shared_ptr< ShaderChannelIface > channel, const std::string & shadersDir )
    : m_channel( channel )
    , m_shaderSource( "" )
    , m_shadersDir( shadersDir )
{
}

// *********************************
//
template< typename ShaderChannelIface >
DefaultFinalizeShaderChannel< ShaderChannelIface >::~DefaultFinalizeShaderChannel   ()
{
}

// *********************************
//
template< typename ShaderChannelIface >
bool    DefaultFinalizeShaderChannel< ShaderChannelIface >::IsReadOnly              () const    //Should also be in IChannel
{
    return m_channel->IsReadOnly();
}

// *********************************
//
template< typename ShaderChannelIface >
void    DefaultFinalizeShaderChannel< ShaderChannelIface >::PostUpdate              ()          //Should also be in IChannel
{
    m_channel->PostUpdate();
}

// *********************************
//
template< typename ShaderChannelIface >
const std::vector< bv::IValueConstPtr > &   DefaultFinalizeShaderChannel< ShaderChannelIface >::GetValues   () const
{
    return m_channel->GetValues();
}

// *********************************
//
template< typename ShaderChannelIface >
bv::IValueConstPtr  DefaultFinalizeShaderChannel< ShaderChannelIface >::GetValue            ( const std::string & name ) const
{
    return m_channel->GetValue( name );
}

// *********************************
//
template< typename ShaderChannelIface >
const std::string &     DefaultFinalizeShaderChannel< ShaderChannelIface >::GetShaderSource () const
{
    return m_shaderSource;
}

// *********************************
//
template< typename ShaderChannelIface >
ITexturesDataConstPtr   DefaultFinalizeShaderChannel< ShaderChannelIface >::GetTexturesData () const
{
    return m_channel->GetTexturesData();
}

// *********************************
//
template< typename ShaderChannelIface >
void                    DefaultFinalizeShaderChannel< ShaderChannelIface >::RegenerateShaderSource( const std::vector< std::string > & uids ) const
{
    m_shaderSource = GetShaderSource( uids );
}

// *********************************
//
template< typename ShaderChannelIface >
const std::vector< std::vector< std::string > > &   DefaultFinalizeShaderChannel< ShaderChannelIface >::GetAcceptedPluginLists  ()
{
    if( ms_acceptedPluginLists.size() == 0 )
    {
        for( unsigned int i = 0; i < GNumLists; ++i )
        {
            std::vector< std::string > entries;
            std::string empty( "" );

            for( unsigned int j = 0; j < GMaxNumEntries; ++j )
            {
                std::string entry( acceptedPluginLists[ i ][ j ] );
                
                if ( entry != empty )
                {
                    entries.push_back( entry );
                }
            }

            ms_acceptedPluginLists.push_back( entries );
        }
    }

    return ms_acceptedPluginLists;
}

// *********************************
//
template< typename ShaderChannelIface >
const std::vector< std::string > &                  DefaultFinalizeShaderChannel< ShaderChannelIface >::GetBaseShaderFileNames  ()
{
    if( ms_baseShaderFileNames.size() == 0 )
    {
        for( unsigned int i = 0; i < GNumLists; ++i )
        {
            ms_baseShaderFileNames.push_back( std::string( baseShaderFileNames[ i ] ) );
        }
    }

    return ms_baseShaderFileNames;
}

// *********************************
//
template< typename ShaderChannelIface >
const std::string &                                 DefaultFinalizeShaderChannel< ShaderChannelIface >::ShaderStorageDirectory  () const
{
    return m_shadersDir;
}

//}

} //model
} //bv
