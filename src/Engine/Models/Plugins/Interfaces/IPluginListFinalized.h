#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IPluginList.h"
#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"


namespace bv { namespace model {

class IPluginListFinalized : public IPluginList
{
public:

    virtual const IFinalizePlugin * GetFinalizePlugin       () const  = 0;

    virtual                         ~IPluginListFinalized   () {}

};

typedef std::shared_ptr< IPluginListFinalized > IPluginListFinalizedPtr;

} //model
} //bv
