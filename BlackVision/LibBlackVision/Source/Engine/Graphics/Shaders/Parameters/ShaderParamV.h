#pragma once


#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/Camera.h"




namespace bv
{


class ShaderParamV : public GenericShaderParam
{
private:

    glm::mat4   m_v;

public:
                            ShaderParamV    ( const std::string & name );
                            ~ShaderParamV   ();

    virtual void            Update          ( RenderableEntity * renderable, Camera * camera ) override;

private:

    virtual const void *    GetValuePtr     () const override;

};


}	// bv
