#include "NRenderLogicCore.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogicImpl.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderOutputChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
void    NRenderLogicCore::Render    ( RenderResult * result, SceneNode * root, NRenderContext * ctx )
{
    if( root )
    {
        // FIXME: default implementation - renders to the first channel only
		auto outputRenderTarget = result->GetActiveRenderTarget( RenderOutputChannelType::ROCT_FIRST_OUTPUT );

        NNodeRenderLogic::Render( root, outputRenderTarget, ctx );
    }
}

} //nrl
} //bv
