#pragma once

#include <string>


namespace bv {

enum class ShaderCategory : int
{
    SC_FULLSCREENEFFECT = 0,
    SC_NODEEFFECTLOGIC,

    SC_TOTAL
};

class ShaderSourceProvider
{
private:

    std::string     m_dir;

private:

    ShaderSourceProvider    ( const std::string & dir );
    ~ShaderSourceProvider   ();

public:

    static const ShaderSourceProvider *     GetProvider ( ShaderCategory category );

    std::string                             ReadShader  ( const std::string & fn ) const;

};

} //bv
