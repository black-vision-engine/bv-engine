#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"
#include "Engine/Graphics/Resources/DataBuffer.h"


namespace bv {

class ITextureDescriptor : public ITextureParams
{
public:

    virtual const char *            GetBits             () const = 0;
    virtual bool                    BitsChanged         () const = 0;
    virtual void                    ResetBitsChanged    () const = 0;
    virtual DataBuffer::Semantic    GetSemantic         () const = 0;

    virtual ~ITextureDescriptor() {}

};

DEFINE_PTR_TYPE(ITextureDescriptor)
DEFINE_CONST_PTR_TYPE(ITextureDescriptor)

} //bv
