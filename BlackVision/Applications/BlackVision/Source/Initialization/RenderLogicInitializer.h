#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogic.h"
#include "BVConfig.h"


namespace bv { namespace nrl {

class RenderedChannelsDataDesc;
class OutputLogicDesc;
class OutputDesc;

class RenderLogicInitializer
{
public:

    static NRenderLogic *   CreateInstance  ( const BVConfig & cfg );

private:

    static void             Initialize          ( RenderedChannelsDataDesc & desc, const BVConfig & cfg );
    static void             Initialize          ( OutputLogicDesc & desc, const BVConfig & cfg );

    static void             InitializeDefaultPrv( OutputDesc & desc, const BVConfig & cfg );
    static void             InitializeDefaultShm( OutputDesc & desc, const BVConfig & cfg );
    static void             InitializeDefaultVid( OutputDesc & desc, const BVConfig & cfg );

};

} // nrl
} // bv
