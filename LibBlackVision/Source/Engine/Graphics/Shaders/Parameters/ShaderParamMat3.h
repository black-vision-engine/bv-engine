#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamMat3 : public GenericShaderParam
{
private:

    const ValueMat3 *   m_valModel;
    
public:

                            ShaderParamMat3     ( const std::string & name, const ValueMat3 * value );
                            ~ShaderParamMat3    ();

    void                    SetModelValue       ( const ValueMat3 * value );

    virtual const void *    GetValuePtr         () const override;

};

} //bv
