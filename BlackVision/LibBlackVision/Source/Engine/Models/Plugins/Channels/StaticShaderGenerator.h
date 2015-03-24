#pragma once

#include "ShaderChannel.h"
#include <map>

namespace bv { namespace model {

// FIXME: temporary, toremove
    const unsigned int GNumLists = 19;
    const unsigned int GMaxNumEntries = 5;
    extern char * acceptedPluginLists[ GNumLists ][ GMaxNumEntries ];
    extern char * baseShaderFileNames[ GNumLists ];
// end of toremove FIXME

    typedef std::map< std::vector< std::string >, std::string >  PluginUIDHashMap;

    class StaticShaderGenerator //: public DefaultFinalizeShaderChannel< IPixelShaderChannel >  // FIXME for God's sake!
    {
        static PluginUIDHashMap    ms_pixelShaderMapping;
        static std::vector< std::vector< std::string > >    ms_acceptedPluginLists;
        static std::vector< std::string >                   ms_baseShaderFileNames;
        std::string                             m_shadersDir;

        //std::string dummy;
        //virtual const std::string &                         GetShaderSource                 () const override { return dummy; }
        //virtual std::string                                 GetShaderSource                 ( const std::vector< std::string > & ) const { return dummy; }

    public:

        StaticShaderGenerator( /*IPixelShaderChannelPtr channel,*/ const std::string & shadersDir ) 
            : 
            //m_channel( channel )
            //, m_shaderSource( "" )
            //, 
            m_shadersDir( shadersDir )
        {
        }

        //virtual RendererContextConstPtr     GetRendererContext  () const { return nullptr; }

        void                InitializePixelShaderMapping       () const
        {
            if( ms_pixelShaderMapping.size() == 0 )
            {
                auto uidLists   = GetAcceptedPluginLists();
                auto baseNames  = GetBaseShaderFileNames();

                assert( uidLists.size() == baseNames.size() );

                for( size_t i = 0; i < uidLists.size(); ++i )
                {
                    ms_pixelShaderMapping[ uidLists[ i ] ] = ShaderStorageDirectory() + baseNames[ i ] + ".frag";
                }
            }
        }

        std::string         GenerateShaderSource( const std::vector< std::string > & uids ) const
        {
            auto it = ms_pixelShaderMapping.find( uids );

            if( it != ms_pixelShaderMapping.end() )
            {
                printf( "Loading pixel shader from: %s\n", it->second.c_str() );
                return ReadShaderContentsFromFile( it->second );
            }

            assert( false );

            return "";
        }

        const std::vector< std::vector< std::string > > &   GetAcceptedPluginLists  () const
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

        const std::vector< std::string > &                  GetBaseShaderFileNames  () const
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

        const std::string &                                 ShaderStorageDirectory  () const
        {
            return m_shadersDir;
        }
    };

} // model
} // bv
