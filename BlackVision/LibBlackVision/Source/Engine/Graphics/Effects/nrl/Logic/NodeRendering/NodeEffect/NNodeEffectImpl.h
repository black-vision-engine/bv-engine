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


    // FIXME: these two methods are added because no valid rendering scheme was designed and right now we mix sorting, blending and inverse z-sorting
    // FIXME: with two queues used to implement that - this suxxx, because it is not consistent and requires query functions as presented belov
    // FIXME: at the very least, a separate query API should be added here, without specifying predefined functions
    virtual void            GetRenderPasses_DIRTY_HACK          ( std::set< const RenderablePass * > * passes ) const;
    virtual bool			IsBlendable_DIRTY_DESIGN_HACK	    () const override;
    virtual bool            IsDepthOverriden_DIRTY_DESIGN_HACK	() const override;
    virtual float			GetDepth_DIRTY_DESIGN_HACK		    () const override;
};

} // nrl
} // bv
