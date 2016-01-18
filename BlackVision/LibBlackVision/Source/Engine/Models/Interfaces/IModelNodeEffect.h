#pragma once

#include "Engine/Models/Interfaces/IModelFullscreenEffect.h"
#include "Engine/Interfaces/IUpdatable.h"

namespace bv { namespace model {

class IModelNodeEffect : public bv::IUpdatable
{
public:

    virtual IModelFullscreenEffectPtr				GetFullscreenEffect	    () const = 0;

    virtual                                         ~IModelNodeEffect       () {};

};

DEFINE_PTR_TYPE(IModelNodeEffect)
DEFINE_CONST_PTR_TYPE(IModelNodeEffect)

} // model
} // bv
