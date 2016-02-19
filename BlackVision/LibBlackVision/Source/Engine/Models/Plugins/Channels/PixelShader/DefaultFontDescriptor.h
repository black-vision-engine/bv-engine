#include "Engine/Models/Interfaces/IFontDescriptor.h"

namespace bv { namespace model {

class DefaultFontDescriptor;
DEFINE_PTR_TYPE(DefaultFontDescriptor)
DEFINE_CONST_PTR_TYPE(DefaultFontDescriptor)

class DefaultFontDescriptor : public IFontDescriptor
{
    std::string                             m_name;
    ITextureDescriptorPtr                   m_atlas;
    ResourceStateModelPtr                   m_stateModel;
public:

    typedef uintptr_t   uid_t;

public:
    DefaultFontDescriptor( ITextureDescriptorPtr atlas, std::string name );
    ~DefaultFontDescriptor();

    virtual uid_t                           GetUID              () const;

    virtual const std::string               GetName             () const override;
    virtual ITextureDescriptorPtr           GetAtlas            () const override;
    virtual model::ResourceStateModelPtr    GetStateModel       () const override;
};

} }
