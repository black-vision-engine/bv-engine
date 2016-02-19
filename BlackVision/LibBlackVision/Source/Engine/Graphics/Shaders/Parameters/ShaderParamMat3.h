#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamMat3 : public GenericShaderParam
{
private:

    glm::mat3				m_val;
    
public:

                            ShaderParamMat3     ( const std::string & name, const glm::mat3 & value = glm::mat3( 0.f ) );

    void                    SetValue			( const glm::mat3 & value );

private:
   
	virtual const void *    GetValuePtr         () const override;

};

} //bv
