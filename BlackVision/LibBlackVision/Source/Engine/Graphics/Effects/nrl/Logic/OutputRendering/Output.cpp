#include "stdafx.h"

#include "Output.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputDesc.h"


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
    { desc; }

    // FIXME: implement

    return nullptr;
}

} //nrl
} //bv
