#pragma once

#include <vector>
#include "System/BasicTypes.h"

namespace bv {

class ITextureDescriptor;
class IAnimationDescriptor;

class ITexturesData
{
public:

    virtual const std::vector< ITextureDescriptor * > &     GetTextures     () const = 0;
    virtual const std::vector< IAnimationDescriptor * > &   GetAnimations   () const = 0;

    virtual ~ITexturesData ()   {}

};

DEFINE_PTR_TYPE(ITexturesData)
DEFINE_CONST_PTR_TYPE(ITexturesData)

} //bv
