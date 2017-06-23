#include "stdafx.h"

#include "Mathematics/glm_inc.h"
#include "ShaderParamNormalMatrixMV.h"


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

        m_normalMat = glm::mat3( glm::transpose( glm::inverse( mv ) ) );
    }
}

} //bv
