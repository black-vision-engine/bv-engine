#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamVec4 : public GenericShaderParam
{
private:

    glm::vec4				m_val;
    
public:

                            ShaderParamVec4     ( const std::string & name, const glm::vec4 & value = glm::vec4( 0.f ) );

    void                    SetValue			( const glm::vec4 & value );

private:
  
	virtual const void *    GetValuePtr         () const override;

};

} //bv
