#pragma once


namespace bv {

class UniformShaderParam;

class IShaderParamUpdater
{
public:

    virtual void Update             ( UniformShaderParam * param ) = 0;
    virtual ~IShaderParamUpdater    ();
};

} //bv
