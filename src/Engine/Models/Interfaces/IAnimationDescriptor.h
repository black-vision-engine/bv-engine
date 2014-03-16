#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"


namespace bv {

class IAnimationDescriptor : public ITextureParams
{
public:

    virtual unsigned int            NumTextures         () const = 0;
    virtual const char *            GetBits             ( unsigned int txNum ) const = 0;

    virtual ~IAnimationDescriptor() {}

};

} //bv
