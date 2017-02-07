#include "stdafxBVApp.h"

#include "RenderLogicInitializer.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/NRenderLogicDesc.h"


namespace bv { namespace nrl {

// *********************************
//
NRenderLogic *   RenderLogicInitializer::CreateInstance ( const BVConfig & cfg )
{
    NRenderLogicDesc desc;

    Initialize( desc.AccessRenderedChannelsDataDesc(), cfg );
    Initialize( desc.AccessOutputLogicDesc(), cfg );

    auto res = NRenderLogic::Create( desc );

    return res;
}

// *********************************
// FIXME: nrl - implement
void            RenderLogicInitializer::Initialize      ( RenderedChannelsDataDesc & desc, const BVConfig & cfg )
{
    { cfg; }

    desc.SetNumTrackedRenderTargets( 2 );

    desc.SetEnabled ( RenderChannelType::RCT_OUTPUT_1 );
    desc.SetDisabled( RenderChannelType::RCT_OUTPUT_2 );
    desc.SetDisabled( RenderChannelType::RCT_OUTPUT_3 );
    desc.SetDisabled( RenderChannelType::RCT_OUTPUT_4 );
}

// *********************************
//
void            RenderLogicInitializer::Initialize      ( OutputLogicDesc & desc, const BVConfig & cfg )
{
    OutputDesc prvDesc;
    OutputDesc shmDesc;
    OutputDesc vidDesc;

    InitializeDefaultPrv( prvDesc, cfg );
    InitializeDefaultShm( shmDesc, cfg );
    InitializeDefaultVid( vidDesc, cfg );

    desc.AppendDesc( prvDesc );
    desc.AppendDesc( shmDesc );
    desc.AppendDesc( vidDesc );
}

// *********************************
//
void             RenderLogicInitializer::InitializeDefaultPrv( OutputDesc & desc, const BVConfig & cfg )
{
}

// *********************************
//
void             RenderLogicInitializer::InitializeDefaultShm( OutputDesc & desc, const BVConfig & cfg )
{
}

// *********************************
//
void             RenderLogicInitializer::InitializeDefaultVid( OutputDesc & desc, const BVConfig & cfg )
{
}

} // nrl
} // bv
