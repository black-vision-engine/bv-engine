#pragma once

#include "CoreDEF.h"

#include "Initialization/RenderLogicInitializer.h"




namespace bv
{

// ***********************
//
class TEST_ACCESSOR( RenderLogicInitializer )
{
public:

    static void             Initialize          ( RenderedChannelsDataDesc & desc, const BVConfig & cfg )   { RenderLogicInitializer::Initialize( desc, cfg ); }
    static void             Initialize          ( OutputLogicDesc & desc, const BVConfig & cfg )            { RenderLogicInitializer::Initialize( desc, cfg ); }
};

}	// bv

