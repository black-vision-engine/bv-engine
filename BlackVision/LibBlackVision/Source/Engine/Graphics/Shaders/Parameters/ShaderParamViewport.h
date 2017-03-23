#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

class ShaderParamViewport : public GenericShaderParam
{
private:

    float       m_lastViewportWidth;
    float       m_lastViewportHeight;

    glm::mat4   m_viewportMatrix;

public:

                            ShaderParamViewport     ( const std::string & name );
                            ~ShaderParamViewport    ();

    virtual void            Update                  ( RenderableEntity * renderable, Camera * camera ) override;

private:
    
	virtual const void *    GetValuePtr             () const override;

};

} //bv
