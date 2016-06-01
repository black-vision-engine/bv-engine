#include "stdafx.h"

#include "ShaderParamMV.h"


namespace bv {

// ********************************
//
ShaderParamMV::ShaderParamMV                ( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT4, name )
    , m_mv( 1.0f ) 
{
}

// ********************************
//
ShaderParamMV::~ShaderParamMV               ()
{
}

// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamMV::GetValuePtr  () const
{
    return &m_mv;
}

// ********************************
//
void            ShaderParamMV::Update       ( RenderableEntity * renderable, Camera * camera )
{
    { camera; } // FIXME: suppress unused variable
    if( IsUpdatable() )
    {
        assert( renderable != nullptr );

        m_mv = camera->GetViewMatrix() * renderable->WorldTransform().Matrix();
    }
}

} //bv
