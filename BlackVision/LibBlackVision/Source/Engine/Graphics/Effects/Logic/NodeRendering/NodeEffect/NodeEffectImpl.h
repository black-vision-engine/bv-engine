#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/NodeEffect.h"


namespace bv { 

class NodeEffectRenderLogic;

class NodeEffectImpl : public NodeEffect
{
private:

    NodeEffectRenderLogic *    m_nodeEffectRenderLogic;

public:
    
                        NodeEffectImpl ( NodeEffectRenderLogic * renderLogic, NodeEffectType type );
                        ~NodeEffectImpl(); 

    virtual void        Render          ( SceneNodeRepr * node, RenderContext * ctx ) override;

    virtual IValuePtr   GetValue        ( const std::string & name ) const override;


    // FIXME: these two methods are added because no valid rendering scheme was designed and right now we mix sorting, blending and inverse z-sorting
    // FIXME: with two queues used to implement that - this suxxx, because it is not consistent and requires query functions as presented belov
    // FIXME: at the very least, a separate query API should be added here, without specifying predefined functions
    virtual void            GetRenderPasses_DIRTY_HACK          ( std::set< const RenderablePass * > * passes ) const;
    virtual bool			IsBlendable_DIRTY_DESIGN_HACK	    () const override;
    virtual bool            IsDepthOverriden_DIRTY_DESIGN_HACK	() const override;
    virtual float			GetDepth_DIRTY_DESIGN_HACK		    () const override;
};


} // bv
