#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"


namespace bv { namespace model {

class IFinalizePlugin : public IPlugin
{
public:

    virtual                         ~IFinalizePlugin    () {}

};

DEFINE_PTR_TYPE(IFinalizePlugin)
DEFINE_CONST_PTR_TYPE(IFinalizePlugin)

} //model
} //bv
