#include "NFinalizeStep.h"


namespace bv { namespace nrl {

// **************************
//
NFinalizeStep::NFinalizeStep            ( NRenderComponentStatePtr initialState )
    : Parent( initialState )
{
}

// **************************
//
void    NFinalizeStep::ReadInputState   ()
{
    // FIXME: implement
}

// **************************
//
void    NFinalizeStep::Apply		    ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    // FIXME: implement
    { nodeRepr; ctx; }
}

// **************************
//
bool    NFinalizeStep::IsFinal          ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return true;
}

} // nrl
} // bv
