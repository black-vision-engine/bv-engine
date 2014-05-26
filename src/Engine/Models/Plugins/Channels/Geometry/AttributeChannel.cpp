#include "AttributeChannel.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"


namespace bv { namespace model
{

// *********************************
//
AttributeChannel::AttributeChannel( const AttributeChannelDescriptor * desc, const std::string & name, bool readOnly )
    : m_desc( desc )
    , m_name( name )
    , m_readOnly( readOnly )
{
    assert( desc != nullptr );
}

// *********************************
//
AttributeChannel::~AttributeChannel ()
{
    //FIXME: not an owner of pointed channel descriptor so don't try to touch it here
}

// *********************************
//
void  AttributeChannel::SetReadOnly             ( bool readOnly )
{
    m_readOnly = readOnly;
}

// *********************************
//
bool  AttributeChannel::IsReadOnly              ()  const
{
    return m_readOnly;
}

// *********************************
//
const IAttributeChannelDescriptor *   AttributeChannel::GetDescriptor           ()  const
{
    return m_desc;
}

// *********************************
//
std::string                                 AttributeChannel::GetName           ()  const
{
    return m_name;
}

// *************************************
//
IAttributeChannel *                         AttributeChannel::GetPositionChannel( const std::vector< IAttributeChannel * > & channels )
{
    if( !channels.empty() )
    {
        // try to guess
        if( channels[ 0 ]->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION )
            return channels[ 0 ];

        for( auto ch : channels )
            if( ch->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION )
                return ch;
    }

    return nullptr;
}

// *************************************
//
IAttributeChannel*                          AttributeChannel::GetUVChannel( const std::vector< IAttributeChannel* >& channels, unsigned int index )
{
    assert( !channels.empty() );
    assert( channels.size() > index );
    assert( channels[index]->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_TEXCOORD );

    return channels[index];
}


} // model
} // bv
