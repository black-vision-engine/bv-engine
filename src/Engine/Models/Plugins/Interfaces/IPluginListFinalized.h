#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IPluginList.h"


namespace bv { namespace model {

class IPluginListFinalized : public IPluginList
{
public:

    virtual const IPlugin * GetFinalizePlugin       () const  = 0;

    virtual                 ~IPluginListFinalized   () {}

};

typedef std::shared_ptr< IPluginListFinalized > IPluginListFinalizedPtr;

} //model
} //bv
