#include "stdafx.h"

#include "NRenderLogicCore.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogicImpl.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderOutputChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
void    NRenderLogicCore::Render    ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx )
{
    for( auto & scene : scenes )
    {
        // FIXME: nrl - retrieve rt for each scene here (for each scene marked as a subgroup)
        auto rt = result->GetActiveRenderTarget( RenderOutputChannelType::ROCT_OUTPUT_1 );

        NNodeRenderLogic::RenderQueued( scene, rt, ctx );
        NNodeRenderLogic::RenderAudio( scene, ctx );
    }
}

} //nrl
} //bv
