#include "stdafx.h"

#define GLM_FORCE_SSE2


#include "ShaderParamNormalMatrixMV.h"

#include "glm/gtx/simd_mat4.hpp"


#include "Memory/MemoryLeaks.h"



namespace bv {

// ********************************
//
ShaderParamNormalMatrixMV::ShaderParamNormalMatrixMV                  ( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT3, name )
    , m_normalMat( 1.0f ) 
{
}

// ********************************
//
ShaderParamNormalMatrixMV::~ShaderParamNormalMatrixMV                 ()
{
}

// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamNormalMatrixMV::GetValuePtr   () const
{
    return &m_normalMat;
}

// ********************************
//
void            ShaderParamNormalMatrixMV::Update        ( RenderableEntity * renderable, Camera * camera )
{
    { renderable; } // FIXME: suppress unused variable

    if( IsUpdatable() )
    {
        assert( camera != nullptr );    { camera; }

        auto mv = camera->GetViewMatrix() * renderable->WorldTransform().Matrix();

        m_normalMat = glm::mat3( glm::mat4_cast(glm::transpose(glm::inverse(glm::detail::fmat4x4SIMD(mv)))) );
    }
}

} //bv
