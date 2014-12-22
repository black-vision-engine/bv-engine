#pragma once

#include "Engine/Interfaces/IShaderDataSource.h"

namespace bv {

class IMultipassShaderDataSource
{
public:

    virtual const IShaderDataSource *   GetVertexShaderDataSource   ( unsigned int i )  const = 0;
    virtual const IShaderDataSource *   GetPixelShaderDataSource    ( unsigned int i )  const = 0;
    virtual const IShaderDataSource *   GetGeometryShaderDataSource ( unsigned int i )  const = 0;

    virtual unsigned int                GetNumPasses                ()                  const = 0;

    virtual ~IMultipassShaderDataSource () {}

};

DEFINE_PTR_TYPE(IMultipassShaderDataSource)
DEFINE_CONST_PTR_TYPE(IMultipassShaderDataSource)

} //bv
