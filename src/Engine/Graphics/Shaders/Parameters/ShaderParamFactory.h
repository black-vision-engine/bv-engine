#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class IValue;

class ShaderParamFactory
{
public:

    static GenericShaderParam *     CreateParameter( const IValue * value );    

};

} //bv
