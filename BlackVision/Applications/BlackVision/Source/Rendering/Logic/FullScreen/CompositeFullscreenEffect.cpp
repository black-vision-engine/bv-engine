#include "CompositeFullscreenEffect.h"

#include "Rendering/Logic/FullScreen/FullscreenEffectGraph.h"


namespace bv {

// ****************************
//
CompositeFullscreenEffect::CompositeFullscreenEffect   ( FullscreenEffectGraph * graph )
    : m_graph( graph )
{
}

// ****************************
//
CompositeFullscreenEffect::~CompositeFullscreenEffect  ()
{
    delete m_graph;
}

// ****************************
//
void    CompositeFullscreenEffect::Render                      ( FullscreenEffectContext * ctx )
{
    { ctx; }

    //TODO: traverse the graph and render everything accordingly
}

} //bv

