#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamBool : public GenericShaderParam
{
private:

    bool                    m_val;
    
public:

                            ShaderParamBool     ( const std::string & name, bool value = false );

    void                    SetValue            ( bool value );

private:
   
    virtual const void *    GetValuePtr         () const override;

};

} //bv
