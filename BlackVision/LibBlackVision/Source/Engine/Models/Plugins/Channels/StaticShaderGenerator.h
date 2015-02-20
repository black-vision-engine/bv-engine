//#include "DefaultFinalizeShaderChannel.inl"
#include "ShaderChannel.h"

namespace bv { namespace model {

class StaticShaderGenerator : public DefaultFinalizeShaderChannel< IPixelShaderChannel > { // FIXME for God's sake!
    
    static PluginUIDHashMap    ms_pixelShaderMapping;

    std::string dummy;
    virtual const std::string &                         GetShaderSource                 () const override { return dummy; }
    virtual std::string                                 GetShaderSource                 ( const std::vector< std::string > & ) const { return dummy; }

public:

    StaticShaderGenerator( IPixelShaderChannelPtr channel, const std::string & shadersDir ) : DefaultFinalizeShaderChannel( channel, shadersDir ) {}
    
    virtual RendererContextConstPtr     GetRendererContext  () const { return nullptr; }

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
};

} // model
} // bv
