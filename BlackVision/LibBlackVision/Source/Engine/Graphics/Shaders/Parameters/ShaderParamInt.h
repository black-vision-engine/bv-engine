#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamInt : public GenericShaderParam
{
private:

    Int32					m_val;
    
public:

							ShaderParamInt		( const std::string & name, Int32 value  = 0 );

    void                    SetValue			( Int32 value );

private:
   
	virtual const void *    GetValuePtr         () const override;

};

} //bv
