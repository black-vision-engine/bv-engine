#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/NNodeEffect.h"


namespace bv { namespace nrl {

class NNodeEffectRenderLogic;

class NNodeEffectImpl : public NNodeEffect
{
private:

    NNodeEffectRenderLogic *    m_nodeEffectRenderLogic;

public:
    
                        NNodeEffectImpl ( NNodeEffectRenderLogic * renderLogic, NNodeEffectType type );
                        ~NNodeEffectImpl(); 

    virtual void        Render          ( SceneNodeRepr * node, NRenderContext * ctx ) override;

    virtual IValuePtr   GetValue        ( const std::string & name ) const override;

};

} // nrl
} // bv
