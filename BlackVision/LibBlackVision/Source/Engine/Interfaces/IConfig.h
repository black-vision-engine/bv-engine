#pragma once

#include <string>

namespace bv
{

class IConfig
{
private:
public:
    virtual     ~IConfig() = 0 {};


    virtual const std::string &         PropertyValue           ( const std::string & key ) const = 0;
};

}	// bv
