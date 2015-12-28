#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamMat2 : public GenericShaderParam
{
private:

    glm::mat2				m_val;
    
public:

                            ShaderParamMat2     ( const std::string & name, const glm::mat2 & value = glm::mat2( 0.f ) );

    void                    SetValue			( const glm::mat2 & value );

private:
   
	virtual const void *    GetValuePtr         () const override;

};

} //bv
