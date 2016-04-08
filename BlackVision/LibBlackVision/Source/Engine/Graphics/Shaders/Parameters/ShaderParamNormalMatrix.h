#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

class ShaderParamNormalMatrix : public GenericShaderParam
{
private:

    glm::mat3   m_normalMat;

public:

                            ShaderParamNormalMatrix    ( const std::string & name );
                            ~ShaderParamNormalMatrix   ();

    virtual void            Update          ( RenderableEntity * renderable, Camera * camera ) override;

private:

    virtual const void *    GetValuePtr     () const override;

};

} //bv
