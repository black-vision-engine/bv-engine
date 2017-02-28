#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"


namespace bv { namespace nrl { 

class OutputDesc;

class OutputFactory
{
public:

    static  Output *    CreateOutput( const OutputDesc & desc );

};

} //nrl
} //bv
