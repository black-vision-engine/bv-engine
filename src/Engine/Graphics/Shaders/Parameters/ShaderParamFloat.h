#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamFloat : public GenericShaderParam
{
private:

    const ValueFloat *   m_valModel;
    
public:

                            ShaderParamFloat    ( const std::string & name, const ValueFloat * value );
                            ~ShaderParamFloat   ();

    virtual const void *    GetValuePtr         () const override;

};

} //bv
