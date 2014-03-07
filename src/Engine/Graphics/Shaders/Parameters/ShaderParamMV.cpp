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
//
const void *    ShaderParamMV::GetValuePtr  () const
{
    return &m_mv;
}

// ********************************
//
void            ShaderParamMV::Update       ( RenderableEntity * renderable, Camera * camera )
{
    assert( renderable != nullptr );

    //FIXME: add proper loop here
    m_mv = renderable->WorldTransforms()[ 0 ].Matrix(); //FIXME: instancing to be added here
}

} //bv
