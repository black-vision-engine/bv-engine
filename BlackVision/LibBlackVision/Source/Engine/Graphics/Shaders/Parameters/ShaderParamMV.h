#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

class ShaderParamMV : public GenericShaderParam
{
private:

    glm::mat4   m_mv;

public:

                            ShaderParamMV   ( const std::string & name );
                            ~ShaderParamMV  ();

    virtual void            Update          ( RenderableEntity * renderable, Camera * camera ) override;

private:

    virtual const void *    GetValuePtr     () const override;

};

} //bv
