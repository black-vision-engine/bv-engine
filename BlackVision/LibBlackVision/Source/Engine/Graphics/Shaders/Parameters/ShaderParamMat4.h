#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamMat4 : public GenericShaderParam
{
private:

    glm::mat4				m_val;
    
public:

                            ShaderParamMat4     ( const std::string & name, const glm::mat4 & value = glm::mat4( 0.f ) );

    void                    SetValue			( const glm::mat4 & value );

private:
 
	virtual const void *    GetValuePtr         () const override;

};

} //bv
