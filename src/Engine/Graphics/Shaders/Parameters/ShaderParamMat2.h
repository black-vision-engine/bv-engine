#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamMat2 : public GenericShaderParam
{
private:

    const ValueMat2 *   m_valModel;
    
public:

                            ShaderParamMat2     ( const std::string & name, const ValueMat2 * value );
                            ~ShaderParamMat2    ();

    virtual const void *    GetValuePtr         () const override;

};

} //bv
