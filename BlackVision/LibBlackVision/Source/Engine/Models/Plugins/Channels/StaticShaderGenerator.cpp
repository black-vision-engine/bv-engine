#include "StaticShaderGenerator.h"

#include "StaticShaderGenerator.inl"

namespace bv { namespace model {

    PluginUIDHashMap    StaticShaderGenerator::ms_pixelShaderMapping;
    std::vector< std::vector< std::string > >    StaticShaderGenerator::ms_acceptedPluginLists;
    std::vector< std::string >                   StaticShaderGenerator::ms_baseShaderFileNames;

    // *********************************
    //
    const std::string                            ReadShaderContentsFromFile             ( const std::string & fileName )
    {
        std::stringstream shaderSource;

        File::Open( fileName ) >> shaderSource;

        return shaderSource.str();
    }


} //model
} //bv
