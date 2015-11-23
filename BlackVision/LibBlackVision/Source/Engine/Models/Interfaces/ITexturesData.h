#pragma once

#include "ITextureDescriptor.h"
#include "IAnimationDescriptor.h"
#include "IFontDescriptor.h"

#include "CoreDEF.h"

namespace bv {

class ITexturesData
{
public:

    virtual const std::vector< ITextureDescriptorPtr > &        GetTextures     () const = 0;
    virtual const std::vector< IAnimationDescriptorPtr > &      GetAnimations   () const = 0;
    virtual const std::vector< IFontDescriptorPtr > &           GetFonts        () const = 0;

    virtual ~ITexturesData ()   {}

};

DEFINE_PTR_TYPE(ITexturesData)
DEFINE_CONST_PTR_TYPE(ITexturesData)

} //bv
