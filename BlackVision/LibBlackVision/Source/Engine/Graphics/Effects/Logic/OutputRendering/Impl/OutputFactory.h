#pragma once

#include "Engine/Graphics/Effects/Logic/OutputRendering/Output.h"


namespace bv { namespace nrl { 

class OutputDesc;

class OutputFactory
{
public:

    static  Output *    CreateOutput( const OutputDesc & desc );

};

} //nrl
} //bv
