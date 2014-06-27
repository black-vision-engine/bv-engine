#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamMat4 : public GenericShaderParam
{
private:

    const ValueMat4 *   m_valModel;
    
public:

                            ShaderParamMat4     ( const std::string & name, const ValueMat4 * value );
                            ~ShaderParamMat4    ();

    void                    SetModelValue       ( const ValueMat4 * value );

    virtual const void *    GetValuePtr         () const override;

};

} //bv
