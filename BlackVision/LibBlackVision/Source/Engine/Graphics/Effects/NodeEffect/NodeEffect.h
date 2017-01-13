#pragma once

#include "Engine/Interfaces/IValue.h"

#include <set>

namespace bv {

enum class NodeEffectType : int
{
    NET_DEFAULT = 0,
    NET_ALPHA_MASK,
    NET_NODE_MASK,
    //NET_IMAGE_MASK,
    NET_WIREFRAME,
    NET_MIX_CHANNELS,
    /*
    
    NET_SHADOW,
    

    NET_BOUNDING_BOX,
    */
	NET_LIGHT_SCATTERING,
	NET_BLUR,
    NET_TOTAL
};

class NodeEffectLogic;
class SceneNodeRepr;

namespace nrl 
{
class NRenderContext;
class NNodeEffectRenderLogic; 
}

class NodeEffect
{
private:

    nrl::NNodeEffectRenderLogic *   m_nrlLogic;

    NodeEffectType                  m_type;

public:

                    NodeEffect                  ( NodeEffectLogic * logic,  NodeEffectType nodeEffectType );
                    ~NodeEffect                 ();

    
    void            Render                      ( SceneNodeRepr * node, nrl::NRenderContext * ctx );

	// FIXME: these two methods are added because no valid rendering scheme was designed and right now we mix sorting, blending and inverse z-sorting
	// FIXME: with two queues used to implement that - this suxxx, because it is not consistent and requires query functions as presented belov
	// FIXME: at the very least, a separate query API should be added here, without specifying predefined functions
	bool			IsBlendable_DIRTY_DESIGN_HACK	() const;
	float			GetDepth_DIRTY_DESIGN_HACK		() const;

    //FIXME: remove when transition is finished - this doesn't seem to be required in this model
    NodeEffectType  GetType                     () const;

    //FIXME: remove when transition is finished - this doesn't seem to be required in this model
    //unsigned int    GetNumValues                () const;

    //FIXME: remove when transition is finished - this doesn't seem to be required in this model
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    //FIXME: remove when transition is finished - this doesn't seem to be required in this model
    IValuePtr       GetValue                    ( const std::string & name ) const;

    void            Update                      ();

};

DEFINE_PTR_TYPE(NodeEffect)
DEFINE_CONST_PTR_TYPE(NodeEffect)

} //bv
