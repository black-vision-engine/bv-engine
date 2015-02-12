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
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamP::GetValuePtr   () const
{
    return &m_p;
}

// ********************************
//
void            ShaderParamP::Update        ( RenderableEntity * renderable, Camera * camera )
{
    { renderable; } // FIXME: suppress unused variable

    if( IsUpdatable() )
    {
        assert( camera != nullptr );

        m_p = camera->GetProjectionMatrix();
    }
}

} //bv
