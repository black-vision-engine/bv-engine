#include "stdafx.h"

#include "FinalizeStep.h"


namespace bv { namespace nrl {

// **************************
//
FinalizeStep::FinalizeStep            ( RenderComponentStatePtr initialState )
    : Parent( initialState )
{
}

// **************************
//
void    FinalizeStep::ReadInputState   ()
{
    // FIXME: implement
}

// **************************
//
void    FinalizeStep::Apply		    ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    // FIXME: implement
    { nodeRepr; ctx; }
}

// **************************
//
bool    FinalizeStep::IsFinal          ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return true;
}

} // nrl
} // bv
