#pragma once

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class ITextureDescriptor;

class IAnimationDescriptor
{
public:

    virtual unsigned int                                GetWidth            () const = 0;
    virtual unsigned int                                GetHeight           () const = 0;
    virtual TextureFormat                               GetFormat           () const = 0;
    virtual TextureWrappingMode                         GetWrappingModeX    () const = 0;
    virtual TextureWrappingMode                         GetWrappingModeY    () const = 0;
    virtual TextureFilteringMode                        GetFilteringMode    () const = 0;

    virtual unsigned int                                NumTextures         () const = 0;
    virtual const char *                                GetBits             ( unsigned int txNum ) const = 0;

    virtual ~IAnimationDescriptor() {}

};

} //model
} //bv
