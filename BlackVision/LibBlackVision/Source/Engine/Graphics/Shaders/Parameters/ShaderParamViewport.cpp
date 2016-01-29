#include "stdafx.h"

#include "ShaderParamViewport.h"


namespace bv {

// ********************************
//
ShaderParamViewport::ShaderParamViewport            ( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT4, name )
    , m_viewportMatrix( 1.0f ) 
{
}

// ********************************
//
ShaderParamViewport::~ShaderParamViewport           ()
{
}

// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamViewport::GetValuePtr    () const
{
    return &m_viewportMatrix;
}

// ********************************
//
void            ShaderParamViewport::Update         ( RenderableEntity * renderable, Camera * camera )
{
    { renderable; } // FIXME: suppress unused variable

    if( IsUpdatable() )
    {
        auto w = camera->GetViewportWidth();
        auto h = camera->GetViewportHeight();

        if ( w != m_lastViewportWidth || h != m_lastViewportHeight )
        {
            m_lastViewportWidth = w;
            m_lastViewportHeight = h;

            float w2 = float( w ) / 2.0f;
            float h2 = float( h ) / 2.0f;

            m_viewportMatrix = glm::mat4(   glm::vec4( w2,  0.0f,  0.0f, 0.0f ),
                                            glm::vec4( 0.0f, h2,   0.0f, 0.0f ),
                                            glm::vec4( 0.0f, 0.0f, 1.0f, 0.0f ),
                                            glm::vec4( w2,   h2,   0.0f, 1.0f ) );
        }
    }
}

} //bv
