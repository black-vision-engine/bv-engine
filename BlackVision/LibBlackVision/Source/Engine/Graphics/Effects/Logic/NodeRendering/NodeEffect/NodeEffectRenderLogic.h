#pragma once

#include <vector>

#include "Engine/Interfaces/IValue.h"


namespace bv { 

class SceneNodeRepr;



class RenderContext;
class NodeEffectRenderPass;

class NodeEffectRenderLogic
{
private:

    std::vector< NodeEffectRenderPass * >   m_passes;
    std::vector< NodeEffectRenderPass * >   m_depthPasses;

    // FIXME: Think about another solution.
    bool    m_useBlend;
    bool    m_overrideDepth;
    float   m_depth;

public:

                    NodeEffectRenderLogic      ( const std::vector< NodeEffectRenderPass * > & passes, bool useBlend = true, bool overrideDepth = false, float depth = 0.0 );
                    NodeEffectRenderLogic      ( const std::vector< NodeEffectRenderPass * > & passes, const std::vector< NodeEffectRenderPass * > & depthPasses, bool useBlend = true, bool overrideDepth = false, float depth = 0.0 );
                    ~NodeEffectRenderLogic     ();

    void            Render                      ( SceneNodeRepr * nodeRepr, RenderContext * ctx );
    void            RenderDepth                 ( SceneNodeRepr * nodeRepr, RenderContext * ctx );

	// FIXME: these two methods are added because no valid rendering scheme was designed and right now we mix sorting, blending and inverse z-sorting
	// FIXME: with two queues used to implement that - this suxxx, because it is not consistent and requires query functions as presented belov
	// FIXME: at the very least, a separate query API should be added here, without specifying predefined functions
    void            GetRenderPasses_DIRTY_HACK          ( std::set< const RenderablePass * > * passes ) const;
	bool			IsBlendable_DIRTY_DESIGN_HACK	    () const;
    bool            IsDepthOverriden_DIRTY_DESIGN_HACK	() const;
	float			GetDepth_DIRTY_DESIGN_HACK		    () const;

    IValuePtr       GetValue                        ( const std::string & name ) const;

private:

    void            SetDefaultDepthPasses           ();
};


} // bv
