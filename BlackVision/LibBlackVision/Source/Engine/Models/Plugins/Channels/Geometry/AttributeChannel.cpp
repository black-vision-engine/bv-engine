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
IAttributeChannelPtr                         AttributeChannel::GetAttrChannel	( const std::vector< IAttributeChannelPtr > & channels, AttributeSemantic semantic, Int32 occuranceIdx )
{
	IAttributeChannelPtr ret = nullptr;
    if( !channels.empty() )
    {
        for( auto ch : channels )
		{
            if( ch->GetDescriptor()->GetSemantic() == semantic )
			{
				if( occuranceIdx == 0 )
				{
					return ch;
				}
				ret = ch;
				occuranceIdx--;
			}
		}
    }

    return ret;
}

} // model
} // bv
