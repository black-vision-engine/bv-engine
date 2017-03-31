#include "stdafx.h"

#include "NEmptyPass.h"


namespace bv { namespace nrl {

// **************************
//
void        NEmptyPass::Apply		    ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
	//Intentionally left empty
    { nodeRepr; ctx; }
}

// **************************
//
void        NEmptyPass::ReadInputState  ()
{
    // Intentionally left empty
}

// **************************
//
bool        NEmptyPass::IsFinal		    ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }
    return false;
}

// **************************
//
bool        NEmptyPass::IsIdle		    ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }
    return true;
}

// **************************
//
IValuePtr   NEmptyPass::GetValue        ( const std::string & name ) const
{
    { name; }
    return nullptr;
}

// **************************
//
void        NEmptyPass::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    { passes; }
}

} // nrl
} // bv
