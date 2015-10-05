#include "SerializationObjects.h"
#include "DeserializeObjectImpl.h"

namespace bv {

DeserializeObject::DeserializeObject( std::string filename, model::TimelineManager* tm )
    : pimpl_( new DeserializeObjectImpl( filename, tm ) )
        { }

DeserializeObject::DeserializeObject( std::istream & in, SizeType numBytes, model::TimelineManager* tm )
    : pimpl_( new DeserializeObjectImpl( in, numBytes, tm ) )
{
}

DeserializeObject::DeserializeObject( DeserializeObjectImpl *pimpl )
    : pimpl_( pimpl )
{
}

DeserializeObject::~DeserializeObject()
{
    delete pimpl_;
}


model::TimelineManager*                                 DeserializeObject::GetTimelineManager() 
{ 
    return pimpl_->m_tm; 
}

std::string                                             DeserializeObject::GetName()
{
    return pimpl_->m_doc->name();
}

std::string                                             DeserializeObject::GetValue( std::string name ) const
{
    auto node = pimpl_->m_doc->first_attribute( name.c_str() );
    assert( node ); // FIXME: error handling
    return node->value();
}

} // bv
