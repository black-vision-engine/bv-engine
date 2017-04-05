#pragma once

#include "CoreDEF.h"

#include <set> // FIXME: nrl - hack
#include "Engine/Interfaces/IValue.h"


namespace bv {

class SceneNodeRepr; 
class RenderablePass; // FIXME: nrl - hack

namespace nrl 
{

enum class NNodeEffectType : int
{
    NNET_DEFAULT = 0,
    NNET_ALPHA_MASK,
    NNET_NODE_MASK,
    NNET_WIREFRAME,
    NNET_MIX_CHANNELS,
	NNET_BLUR,
	NNET_LIGHT_SCATTERING,
	NNET_SHADOW,
    NNET_Z_SORT,
	NNET_GLOW,
	NNET_SOFT_MASK,

    NNET_TOTAL
};

class NRenderContext;

class NNodeEffect
{
private:

    NNodeEffectType     m_type;

protected:
    
                        NNodeEffect     ( NNodeEffectType type );

public:

    virtual             ~NNodeEffect    ();

    virtual void        Render          ( SceneNodeRepr * node, NRenderContext * ctx ) = 0;

    NNodeEffectType     GetType         () const;

    virtual IValuePtr   GetValue        ( const std::string & name ) const = 0;


    // FIXME: these two methods are added because no valid rendering scheme was designed and right now we mix sorting, blending and inverse z-sorting
    // FIXME: with two queues used to implement that - this suxxx, because it is not consistent and requires query functions as presented belov
    // FIXME: at the very least, a separate query API should be added here, without specifying predefined functions
    virtual void            GetRenderPasses_DIRTY_HACK          ( std::set< const RenderablePass * > * passes ) const = 0;
    virtual bool			IsBlendable_DIRTY_DESIGN_HACK	    () const = 0;
    virtual bool            IsDepthOverriden_DIRTY_DESIGN_HACK	() const = 0;
    virtual float			GetDepth_DIRTY_DESIGN_HACK		    () const = 0;
};

DEFINE_PTR_TYPE(NNodeEffect)
DEFINE_CONST_PTR_TYPE(NNodeEffect)

} // nrl
} // bv
