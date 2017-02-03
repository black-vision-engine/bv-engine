#include "stdafx.h"

#include "NRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogic::~NRenderLogic                     ()
{
}

// **************************
// FIXME: implement
NRenderLogic *           NRenderLogic::Create   ( unsigned int width, unsigned int height )
{
    NRenderLogicDesc desc;

    { width; height; }

    return Create( desc );
}

// **************************
// FIXME: implement
NRenderLogic *           NRenderLogic::Create   ( const NRenderLogicDesc & desc )
{
    { desc; }

    return nullptr;
}

} //nrl
} //bv
