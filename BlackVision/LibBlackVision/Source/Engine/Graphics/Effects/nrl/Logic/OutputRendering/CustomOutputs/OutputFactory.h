#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"


namespace bv { namespace nrl { 

class OutputDesc;

Output *    CreateOutput( const OutputDesc & desc );

} //nrl
} //bv
