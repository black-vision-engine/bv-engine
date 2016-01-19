#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"


namespace bv { namespace model {

class ModelNodeEffect : public IModelNodeEffect
{
protected:

    IModelFullscreenEffectPtr               m_fullscreenEffect;

public:

                                            ModelNodeEffect     ( IModelFullscreenEffectPtr fullscreenEffect );

    virtual void                            Update              ( TimeType t ) override;

    IModelFullscreenEffectPtr		        GetFullscreenEffect	() const override;

    static IModelNodeEffectPtr              Create              ( IModelFullscreenEffectPtr fullscreenEffect );

};

} // model
} // bv
