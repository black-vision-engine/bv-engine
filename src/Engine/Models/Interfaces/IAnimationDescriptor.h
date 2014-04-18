#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"


namespace bv {

class IAnimationDescriptor : public ITextureParams
{
public:

    virtual unsigned int            NumTextures         () const = 0;
    virtual const char *            GetBits             ( unsigned int txNum ) const = 0;

    virtual bool                    BitsChanged         () const = 0;
    virtual bool                    BitsChanged         ( unsigned int frameNum ) const = 0;
    virtual void                    ResetBitsChanged    ( unsigned int frameNum ) = 0;

    virtual unsigned int            CurrentFrame        () const = 0;
    virtual unsigned int            PreviousFrame       () const = 0;

    virtual ~IAnimationDescriptor() {}

};

} //bv
