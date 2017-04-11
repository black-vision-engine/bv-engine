#include "stdafx.h"

#include "EmptyPass.h"


namespace bv { 

// **************************
//
void        EmptyPass::Apply		    ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
	//Intentionally left empty
    { nodeRepr; ctx; }
}

// **************************
//
void        EmptyPass::ReadInputState  ()
{
    // Intentionally left empty
}

// **************************
//
bool        EmptyPass::IsFinal		    ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }
    return false;
}

// **************************
//
bool        EmptyPass::IsIdle		    ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }
    return true;
}

// **************************
//
IValuePtr   EmptyPass::GetValue        ( const std::string & name ) const
{
    { name; }
    return nullptr;
}

// **************************
//
void        EmptyPass::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    { passes; }
}


} // bv
