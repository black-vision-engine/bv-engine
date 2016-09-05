#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamVec3 : public GenericShaderParam
{
private:

    glm::vec3				m_val;
    
public:

                            ShaderParamVec3     ( const std::string & name, const glm::vec3 & value = glm::vec3( 0.f ) );

    void                    SetValue			( const glm::vec3 & value );

private:
   
	virtual const void *    GetValuePtr         () const override;

};

} //bv
