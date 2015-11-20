#include "Engine/Models/Interfaces/IFontDescriptor.h"

namespace bv { namespace model {

class DefaultFontDescriptor;
DEFINE_PTR_TYPE(DefaultFontDescriptor)
DEFINE_CONST_PTR_TYPE(DefaultFontDescriptor)

class DefaultFontDescriptor : public IFontDescriptor
{
    ITextureDescriptorPtr                   m_atlas;
    ResourceStateModelPtr                   m_stateModel;
public:

    typedef uintptr_t   uid_t;

public:
    DefaultFontDescriptor( ITextureDescriptorPtr atlas );
    ~DefaultFontDescriptor();

    virtual uid_t                           GetUID              () const;

    virtual ITextureDescriptorPtr           GetAtlas            () const;
    virtual model::ResourceStateModelPtr    GetStateModel       () const;
};

} }
