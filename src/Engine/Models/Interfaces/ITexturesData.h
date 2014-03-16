#pragma once

#include <vector>


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

} //bv
