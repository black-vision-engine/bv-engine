#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class IValue;

class ShaderParamFactory
{
public:

    static GenericShaderParam *     CreateGenericParameter          ( const IValue * value );
    static GenericShaderParam *     CreateGenericParameter          ( const std::string & name, ParamType paramType );

    static GenericShaderParam *     CreateVParameter                ();
    static GenericShaderParam *     CreateMParameter                ();
    static GenericShaderParam *     CreateMVPParameter              ();
    static GenericShaderParam *     CreateMVParameter               ();
    static GenericShaderParam *     CreatePParameter                ();
    static GenericShaderParam *     CreateNormalMatrixMVParameter   ();
    static GenericShaderParam *     CreateNormalMatrixMParameter    ();
    static GenericShaderParam *     CreateViewportMatrixParameter   ();

};

} //bv
