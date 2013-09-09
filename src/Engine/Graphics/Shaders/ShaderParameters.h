#pragma once

#include <vector>

#include "ShaderParam.h"

namespace bv {

class Shader;

class ShaderParameters
{
private:

    typedef std::vector< GenericShaderParam * > TShaderParamVec;

private:

    TShaderParamVec m_shaderParams;    
    Shader *        m_shader;

    //TODO: add texture servicing

public:

    void    SetOwner                        ( Shader * owner );

    bool    IsRegistered                    ( const std::string & name ) const;

    void    RegisterParameter               ( GenericShaderParam * param );
    int     NumParameters                   () const;

    GenericShaderParam * GetParam           ( int id );
    GenericShaderParam * GetParam           ( const std::string& name );

    void                 SetValue            ( const std::string& name , const char* pBuf);

    void                 Update             ( /* ???? */ );

    ~ShaderParameters                       ();

private:

    GenericShaderParam * FindParam          ( const std::string & name ) const;

};

}
