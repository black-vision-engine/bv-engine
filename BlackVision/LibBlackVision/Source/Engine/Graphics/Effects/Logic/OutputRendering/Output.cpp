#include "stdafx.h"

#include "Output.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/OutputDesc.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/OutputFactory.h"


namespace bv { 

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


} //bv
