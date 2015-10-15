#pragma once

#include "CoreDEF.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Types//Enums.h"

namespace bv { namespace model {

class IModelNodeEffect : public bv::IUpdatable
{
public:

    virtual NodeEffectType  GetType             () const = 0;

    virtual                 ~IModelNodeEffect   () {};

};

} // model
} // bv
