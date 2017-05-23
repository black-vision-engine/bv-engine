#pragma once

#include "Engine/Graphics/Effects/Logic/RenderLogic.h"
#include "BVConfig.h"


namespace bv { 

class RenderedChannelsDataDesc;
class OutputLogicDesc;
class OutputDesc;

class RenderLogicInitializer
{
public:

    static RenderLogic *   CreateInstance  ( const BVConfig & cfg );

private:

    static void             Initialize          ( RenderedChannelsDataDesc & desc, const BVConfig & cfg );
    static void             Initialize          ( OutputLogicDesc & desc, const BVConfig & cfg );

    static void             InitializeDefaultPrv( OutputDesc & desc, const BVConfig & cfg );
    static void             InitializeDefaultShm( OutputDesc & desc, const BVConfig & cfg );
    static void             InitializeDefaultVid( OutputDesc & desc, const BVConfig & cfg );
    static void             InitializeDefaultAVFile( OutputDesc & desc, const BVConfig & cfg );

};


} // bv
