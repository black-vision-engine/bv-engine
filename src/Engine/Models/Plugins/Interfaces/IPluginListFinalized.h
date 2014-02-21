#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IPluginList.h"


namespace bv { namespace model {

class IPluginListFinalized : public IPluginList
{
public:

    virtual ~IPluginListFinalized   () {}

};

} //model
} //bv
