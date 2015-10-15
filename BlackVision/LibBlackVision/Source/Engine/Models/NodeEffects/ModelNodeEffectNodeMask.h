#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"


namespace bv { namespace model {

class ModelNodeEffectNodeMask : public IModelNodeEffect
{
public:

    virtual NodeEffectType  GetType() const override;

};

} // model
} // bv
