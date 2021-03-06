#include "stdafx.h"

#include "NodeEffectRenderLogic.h"

#include <cassert>

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/NodeEffectRenderPass.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/FinalizePass.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/DepthImpl/DefaultDepthFinalizeStep.h"



namespace bv { 

// *********************************
//
NodeEffectRenderLogic::NodeEffectRenderLogic      ( const std::vector< NodeEffectRenderPass * > & passes, bool useBlend, bool overrideDepth, float depth )
    : m_passes( passes )
    , m_useBlend( useBlend )
    , m_overrideDepth( overrideDepth )
    , m_depth( depth )
{
    for( auto pass : m_passes )
    {
		pass;
        assert( pass != nullptr );
    }

    assert( passes.size() > 0 );

    SetDefaultDepthPasses();
}

// ***********************
//
NodeEffectRenderLogic::NodeEffectRenderLogic    ( const std::vector< NodeEffectRenderPass * > & passes, const std::vector< NodeEffectRenderPass * > & depthPasses, bool useBlend, bool overrideDepth, float depth )
    : m_passes( passes )
    , m_depthPasses( depthPasses )
    , m_useBlend( useBlend )
    , m_overrideDepth( overrideDepth )
    , m_depth( depth )
{
    for( auto pass : m_passes )
    {
        pass;
        assert( pass != nullptr );
    }

    assert( passes.size() > 0 );

    for( auto pass : m_depthPasses )
    {
        pass;
        assert( pass != nullptr );
    }
}

// *********************************
//
NodeEffectRenderLogic::~NodeEffectRenderLogic     ()
{
    for( auto pass: m_passes )
    {
        delete pass;
    }

    for( auto pass : m_depthPasses )
    {
        delete pass;
    }
}

// *********************************
// FIXME: make sure that no more bookkeeping has to take place in this class
void            NodeEffectRenderLogic::Render  ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    //FIXME: this is no the best protocol to find out whether more passes are to be applied but should suffice right now - IsIdle should be used as well
    for( auto pass : m_passes )
    {
        pass->ReadInputState();

        if( !pass->IsIdle( nodeRepr ) )
        {
            pass->Apply( nodeRepr, ctx );
        }

        if( pass->IsFinal( nodeRepr ) )
        {
            return;
        }
    }    
}

// ***********************
//
void            NodeEffectRenderLogic::RenderDepth                          ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    // This is the same logic as in render function but calls depth passes.
    for( auto pass : m_depthPasses )
    {
        pass->ReadInputState();

        if( !pass->IsIdle( nodeRepr ) )
        {
            pass->Apply( nodeRepr, ctx );
        }

        if( pass->IsFinal( nodeRepr ) )
        {
            return;
        }
    }
}

// *********************************
//
void            NodeEffectRenderLogic::GetRenderPasses_DIRTY_HACK          ( std::set< const RenderablePass * > * passes ) const
{
    for ( auto & pass : m_passes )
    {
        pass->AppendRenderPasses_DIRTY_HACK( passes );
    }
}

// *********************************
//
bool			NodeEffectRenderLogic::IsBlendable_DIRTY_DESIGN_HACK	    () const
{
	return m_useBlend;
}

// ***********************
//
bool            NodeEffectRenderLogic::IsDepthOverriden_DIRTY_DESIGN_HACK  () const
{
    return m_overrideDepth;
}

// *********************************
//
float			NodeEffectRenderLogic::GetDepth_DIRTY_DESIGN_HACK          () const
{
	return m_depth;
}

// *********************************
//
IValuePtr       NodeEffectRenderLogic::GetValue                            ( const std::string & name ) const
{
    IValuePtr res = nullptr;

    for( auto & pass : m_passes )
    {
        res = pass->GetValue( name );

        if( res )
        {
            break;
        }
    }

    return res;
}

// ***********************
//
void            NodeEffectRenderLogic::SetDefaultDepthPasses                ()
{
    // Sets default pass, which renders whole subtree depth.
    auto finalizeStep = new DefaultDepthFinalizeStep();
    auto finPass = new FinalizePass( finalizeStep );

    m_depthPasses.push_back( finPass );
}

 
} //bv
