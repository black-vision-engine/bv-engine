#include "ModelNodeEffect.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffect::ModelNodeEffect  ( IModelFullscreenEffectPtr fullscreenEffect )
    : m_fullscreenEffect( fullscreenEffect )
{
}

// ********************************
//
void                                        ModelNodeEffect::Update     ( TimeType t )
{
    m_fullscreenEffect->Update( t );
}

// ********************************
//
IModelFullscreenEffectPtr                   ModelNodeEffect::GetFullscreenEffect    () const
{
    return m_fullscreenEffect;
}

// ********************************
//
IModelNodeEffectPtr                         ModelNodeEffect::Create         ( IModelFullscreenEffectPtr fullscreenEffect )
{
    return std::make_shared< ModelNodeEffect >( fullscreenEffect );
}

} // model
} // bv
