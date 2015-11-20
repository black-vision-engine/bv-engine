#pragma once

#include "ITextureDescriptor.h"

namespace bv {

class IFontDescriptor
{
public:

    typedef uintptr_t   uid_t;

public:

    virtual uid_t                           GetUID              () const = 0;

    virtual ITextureDescriptorPtr           GetAtlas            () const = 0;
    virtual model::ResourceStateModelPtr    GetStateModel       () const = 0;

    virtual ~IFontDescriptor() {}

};

DEFINE_PTR_TYPE(IFontDescriptor)
DEFINE_CONST_PTR_TYPE(IFontDescriptor)

} //bv
