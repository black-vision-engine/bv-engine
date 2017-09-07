#pragma once

#include "CoreDEF.h"

#include <set> // FIXME: nrl - hack

#include "Engine/Interfaces/IValue.h"


namespace bv {

class SceneNodeRepr; 
class RenderablePass; // FIXME: nrl - hack

enum class NodeEffectType : int
{
    NET_DEFAULT = 0,
    NET_ALPHA_MASK,
    NET_NODE_MASK,
    NET_WIREFRAME,
    NET_MIX_CHANNELS,
	NET_BLUR,
	NET_LIGHT_SCATTERING,
	NET_SHADOW,
    NET_Z_SORT,
	NET_GLOW,
	NET_SOFT_MASK,
    NET_COLOR_GRADE,

    NET_TOTAL
};

class RenderContext;

class NodeEffect
{
private:

    NodeEffectType      m_type;

protected:
    
                        NodeEffect     ( NodeEffectType type );

public:

    virtual             ~NodeEffect    ();

    virtual void        Render          ( SceneNodeRepr * node, RenderContext * ctx ) = 0;
    virtual void        RenderDepth     ( SceneNodeRepr * node, RenderContext * ctx ) = 0;

    NodeEffectType      GetType         () const;

    virtual IValuePtr   GetValue        ( const std::string & name ) const = 0;


    // FIXME: these two methods are added because no valid rendering scheme was designed and right now we mix sorting, blending and inverse z-sorting
    // FIXME: with two queues used to implement that - this suxxx, because it is not consistent and requires query functions as presented belov
    // FIXME: at the very least, a separate query API should be added here, without specifying predefined functions
    virtual void            GetRenderPasses_DIRTY_HACK          ( std::set< const RenderablePass * > * passes ) const = 0;
    virtual bool			IsBlendable_DIRTY_DESIGN_HACK	    () const = 0;
    virtual bool            IsDepthOverriden_DIRTY_DESIGN_HACK	() const = 0;
    virtual float			GetDepth_DIRTY_DESIGN_HACK		    () const = 0;
};

DEFINE_PTR_TYPE(NodeEffect)
DEFINE_CONST_PTR_TYPE(NodeEffect)

} // bv
