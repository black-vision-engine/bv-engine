#include "DefaultFontDescriptor.h"

namespace bv { namespace model {

DefaultFontDescriptor::DefaultFontDescriptor( ITextureDescriptorPtr atlas, std::string name )
    : m_atlas( atlas )
    , m_stateModel( std::make_shared< ResourceStateModel >() )
    , m_name( name )
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
const std::string               DefaultFontDescriptor::GetName             () const
{
    return m_name;
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
