#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class IValue;

class ShaderParamFactory
{
public:

    static GenericShaderParam *     CreateGenericParameter  ( const IValue * value );    

    static GenericShaderParam *     CreateMVPParameter      ();
    static GenericShaderParam *     CreateMVParameter       ();
    static GenericShaderParam *     CreatePParameter        ();

};

} //bv
