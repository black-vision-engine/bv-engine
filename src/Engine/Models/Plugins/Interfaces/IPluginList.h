#pragma once

#include <memory>
#include <vector>

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"


namespace bv { namespace model {

class IPluginList
{
public:

    virtual ~IPluginList    () {}

};

typedef std::shared_ptr< IPluginList > IPluginListPtr;
 
} //model
} //bv
