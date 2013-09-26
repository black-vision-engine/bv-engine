#include "Resource.h"

namespace bv { namespace model {

const std::string&            Resource::GetName     () const
{
    return m_name;
}

const std::string&            Resource::GetFilePath () const
{
    return m_filePath;
}

Resource::Resource( const std::string& name, const std::string& filePath )
    : m_name(name)
    , m_filePath(filePath)
{}


ResourceExtraData::ResourceExtraData( ResourceExtraKind kind )
    : m_kind( kind )
{}

} // model
} // bv