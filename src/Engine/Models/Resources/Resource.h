#pragma once

#include "IResource.h"

namespace bv { namespace model {

class Resource : public IResource
{
private:

    std::string             m_name;
    std::string             m_filePath;

public:

    virtual const std::string&            GetName     () const;
    virtual const std::string&            GetFilePath () const;

    Resource( const std::string& name, const std::string& filePath );
    virtual ~Resource(){}
};

} // model
} // bv