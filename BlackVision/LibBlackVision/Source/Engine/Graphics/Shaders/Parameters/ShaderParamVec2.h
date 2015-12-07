#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamVec2 : public GenericShaderParam
{
private:

    glm::vec2				m_val;
    
public:

                            ShaderParamVec2     ( const std::string & name, const glm::vec2 & value = glm::vec2( 0.f ) );

    void                    SetValue			( const glm::vec2 & value );

private:

    virtual const void *    GetValuePtr         () const override;

};

} //bv
