#pragma once

#include <string>

namespace bv { namespace model
{

class IResource
{
public:
    virtual const std::string&            GetName     () const = 0;
    virtual const std::string&            GetFilePath () const = 0;
    virtual ~IResource(){}
};

} // model
} // bv