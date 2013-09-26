#include "VertexAttributeChannel.h"

#include <cassert>

#include "Engine\Models\Plugins\Channels\VertexAttributeChannelDescriptor.h"

namespace bv { namespace model
{

// *********************************
//
VertexAttributeChannel::VertexAttributeChannel( const VertexAttributeChannelDescriptor * desc, const std::string & name, bool readOnly )
    : m_desc( desc )
    , m_name( name )
    , m_readOnly( readOnly )
{
    assert( desc != nullptr );
}

// *********************************
//
VertexAttributeChannel::~VertexAttributeChannel ()
{
    //FIXME: not an owner of pointed channel descriptor so don't try to touch it here
}

// *********************************
//
void  VertexAttributeChannel::SetReadOnly             ( bool readOnly )
{
    m_readOnly = readOnly;
}

// *********************************
//
bool  VertexAttributeChannel::IsReadOnly              ()  const
{
    return m_readOnly;
}

// *********************************
//
const IVertexAttributeChannelDescriptor *   VertexAttributeChannel::GetDescriptor           ()  const
{
    return m_desc;
}

// *********************************
//
std::string                                 VertexAttributeChannel::GetName                 ()  const
{
    return m_name;
}

} // model
} // bv
