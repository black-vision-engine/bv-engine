#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"


namespace bv { namespace model {

class IFinalizePlugin : public IPlugin
{
public:

    virtual                         ~IFinalizePlugin    () {}
    virtual bool                    IsValid             () = 0;

};

DEFINE_PTR_TYPE(IFinalizePlugin)
DEFINE_CONST_PTR_TYPE(IFinalizePlugin)

} //model
} //bv
