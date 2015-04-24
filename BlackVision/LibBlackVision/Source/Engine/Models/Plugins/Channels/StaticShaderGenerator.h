#pragma once

#include "ShaderChannel.h"
#include <map>

namespace bv { namespace model {

    class StaticShaderGenerator
    {
        std::string                             m_shadersDir;
        std::string                             m_shaderExtension;
    public:
        StaticShaderGenerator( const std::string & shadersDir, const std::string & shaderExtension ) 
            : m_shadersDir( shadersDir )
            , m_shaderExtension( shaderExtension )
        {
        }

        std::string         GenerateShaderSource( const std::vector< std::string > & uids ) const;
        std::string         GenerateFilename( const std::vector< std::string > & uids ) const;
        std::string         UID2Abbrv( const std::string uid ) const;
    };

} // model
} // bv
