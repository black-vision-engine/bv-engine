#pragma once

namespace bv {

class GenericShaderParam;

class IShaderParamUpdater
{
public:

    virtual void Update( GenericShaderParam * param ) = 0;

    virtual ~IShaderParamUpdater() {}
};

class FakeUpdater : public IShaderParamUpdater
{
    virtual void Update( GenericShaderParam * param );
};

}