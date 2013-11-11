#pragma once

#include <string>
#include <vector>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"


namespace bv { namespace model
{

class IValue;
class IParameter;

class IShaderChannel : public IChannel
{
public:
    virtual const std::string&                      GetShaderFile               () const = 0;

    virtual const std::vector<IValue*> &            GetValuesList               () const = 0;
    virtual const std::vector<IParameter*> &        GetParametersList           () const = 0;

    virtual                         ~IShaderChannel(){};
};

} // model
} // bv