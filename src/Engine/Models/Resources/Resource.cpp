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

} // model
} // bv