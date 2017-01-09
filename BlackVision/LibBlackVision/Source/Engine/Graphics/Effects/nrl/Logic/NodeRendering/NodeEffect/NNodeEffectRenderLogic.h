#pragma once

#include <vector>

#include "Engine/Interfaces/IValue.h"


namespace bv { 

class SceneNodeRepr;

namespace nrl {

class NRenderContext;
class NNodeEffectRenderPass;

class NNodeEffectRenderLogic
{
private:

    std::vector< NNodeEffectRenderPass * > m_passes;

public:

                    NNodeEffectRenderLogic      ( const std::vector< NNodeEffectRenderPass * > & passes );
                    ~NNodeEffectRenderLogic     ();

    void            Render                      ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );

	// FIXME: these two methods are added because no valid rendering scheme was designed and right now we mix sorting, blending and inverse z-sorting
	// FIXME: with two queues used to implement that - this suxxx, because it is not consistent and requires query functions as presented belov
	// FIXME: at the very least, a separate query API should be added here, without specifying predefined functions
	bool			IsBlendable_DIRTY_DESIGN_HACK	() const;
	float			GetDepth_DIRTY_DESIGN_HACK		() const;

    IValuePtr       GetValue                        ( const std::string & name ) const;

};

} // nrl
} // bv
