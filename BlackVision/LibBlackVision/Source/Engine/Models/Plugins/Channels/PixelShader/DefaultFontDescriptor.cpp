#include "DefaultFontDescriptor.h"

namespace bv { namespace model {

DefaultFontDescriptor::DefaultFontDescriptor( ITextureDescriptorPtr atlas )
    : m_atlas( atlas )
    , m_stateModel( std::make_shared< ResourceStateModel >() )
{
}

DefaultFontDescriptor::~DefaultFontDescriptor()
{
}

DefaultFontDescriptor::uid_t                           DefaultFontDescriptor::GetUID              () const
{
    assert( false );
    return 0;
}

ITextureDescriptorPtr           DefaultFontDescriptor::GetAtlas            () const
{
    return m_atlas;
}

model::ResourceStateModelPtr    DefaultFontDescriptor::GetStateModel       () const
{
    return m_stateModel;
}

} }
