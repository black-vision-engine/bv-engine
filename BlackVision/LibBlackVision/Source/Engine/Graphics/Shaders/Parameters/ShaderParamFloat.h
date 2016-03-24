#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamFloat : public GenericShaderParam
{
private:

    Float32					m_val;
    
public:

                            ShaderParamFloat    ( const std::string & name, Float32 value = 0.f );

    void                    SetValue			( Float32 value );

private:

    virtual const void *    GetValuePtr         () const override;

};

} //bv
