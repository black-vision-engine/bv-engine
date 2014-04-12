#pragma once

#include <memory>
#include <string>

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class IPluginResourceDescr
{
public:

    virtual const std::string &     GetName         () const = 0;
    virtual PluginResourceType      GetResourceType () const = 0;

    virtual ~IPluginResourceDescr() {}

};

typedef std::shared_ptr< IPluginResourceDescr > IPluginResourceDescrPtr;

} //model
} //bv
