#include "ShaderParameters.h"

#include <cassert>


namespace bv {

// *******************************
//
ShaderParameters::ShaderParameters                          ()
{
}

// *******************************
//
ShaderParameters::~ShaderParameters                         ()
{
    for( auto param : m_shaderParams )
    {
        delete param;
    }
}

// ***************************
//
void                ShaderParameters::UpdateParameters      ()
{
    for( auto param : m_shaderParams )
    {
        param->Update();
    }
}

// ***************************
//
void    ShaderParameters::AddParameter                      ( GenericShaderParam * param )
{
    m_shaderParams.push_back( param );
}

} //bv
