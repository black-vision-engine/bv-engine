#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IPluginList.h"
#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"


namespace bv { namespace model {

class IPluginListFinalized : public IPluginList
{
public:

    virtual IFinalizePluginConstPtr GetFinalizePlugin       () const  = 0;

    virtual                         ~IPluginListFinalized   () {}

};

DEFINE_PTR_TYPE(IPluginListFinalized)

} //model
} //bv
