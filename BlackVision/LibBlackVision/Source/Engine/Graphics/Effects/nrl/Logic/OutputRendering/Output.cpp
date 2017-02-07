#include "stdafx.h"

#include "Output.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputDesc.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/OutputFactory.h"


namespace bv { namespace nrl {

// *********************************
//
Output::~Output ()
{
}

// *********************************
//
Output *     Output::Create ( const OutputDesc & desc )
{
    auto res = OutputFactory::CreateOutput( desc );

    return res;
}

} //nrl
} //bv
