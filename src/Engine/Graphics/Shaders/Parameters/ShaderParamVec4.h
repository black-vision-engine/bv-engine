#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamVec4 : public GenericShaderParam
{
private:

    const ValueVec4 *   m_valModel;
    
public:

                            ShaderParamVec4     ( const std::string & name, const ValueVec4 * value );
                            ~ShaderParamVec4    ();

    virtual const void *    GetValuePtr         () const override;

};

} //bv
