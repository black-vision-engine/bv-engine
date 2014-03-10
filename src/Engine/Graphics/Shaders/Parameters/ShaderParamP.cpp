#include "ShaderParamP.h"


namespace bv {

// ********************************
//
ShaderParamP::ShaderParamP                  ( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT4, name )
    , m_p( 1.0f ) 
{
}

// ********************************
//
ShaderParamP::~ShaderParamP                 ()
{
}

// ********************************
//
const void *    ShaderParamP::GetValuePtr   () const
{
    return &m_p;
}

// ********************************
//
void            ShaderParamP::Update        ( RenderableEntity * renderable, Camera * camera )
{
    if( IsUpdateble() )
    {
        assert( camera != nullptr );

        m_p = camera->GetProjectionMatrix();
    }
}

} //bv
