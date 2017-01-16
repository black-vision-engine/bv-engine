#include "stdafx.h"

#include "NNodeEffectRenderLogic.h"

#include <cassert>

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/NNodeEffectRenderPass.h"


namespace bv { namespace nrl {

// *********************************
//
NNodeEffectRenderLogic::NNodeEffectRenderLogic      ( const std::vector< NNodeEffectRenderPass * > & passes, bool useBlend, bool overrideDepth, float depth )
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
}

// *********************************
//
NNodeEffectRenderLogic::~NNodeEffectRenderLogic     ()
{
    for( auto pass: m_passes )
    {
        delete pass;
    }
}

// *********************************
// FIXME: make sure that no more bookkeeping has to take place in this class
void            NNodeEffectRenderLogic::Render  ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
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

// *********************************
//
bool			NNodeEffectRenderLogic::IsBlendable_DIRTY_DESIGN_HACK	() const
{
	return m_useBlend;
}

// ***********************
//
bool            NNodeEffectRenderLogic::IsDepthOverriden_DIRTY_DESIGN_HACK() const
{
    return m_overrideDepth;
}

// *********************************
//
float			NNodeEffectRenderLogic::GetDepth_DIRTY_DESIGN_HACK      () const
{
	// FIXME: implement
	/*assert( false );*/

	return m_depth;
}

// *********************************
//
IValuePtr       NNodeEffectRenderLogic::GetValue                        ( const std::string & name ) const
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

} //nrl 
} //bv
