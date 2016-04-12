#include "stdafx.h"

#include "ShaderParamNormalMatrix.h"


namespace bv {

// ********************************
//
ShaderParamNormalMatrix::ShaderParamNormalMatrix                  ( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT3, name )
    , m_normalMat( 1.0f ) 
{
}

// ********************************
//
ShaderParamNormalMatrix::~ShaderParamNormalMatrix                 ()
{
}

// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamNormalMatrix::GetValuePtr   () const
{
    return &m_normalMat;
}

// ********************************
//
void            ShaderParamNormalMatrix::Update        ( RenderableEntity * renderable, Camera * /*camera*/ )
{
    { renderable; } // FIXME: suppress unused variable

    if( IsUpdatable() )
    {
        assert( camera != nullptr );

        auto mv = renderable->WorldTransform().Matrix();
        m_normalMat = glm::mat3( glm::transpose( glm::inverse( mv ) ) );
    }
}

} //bv
