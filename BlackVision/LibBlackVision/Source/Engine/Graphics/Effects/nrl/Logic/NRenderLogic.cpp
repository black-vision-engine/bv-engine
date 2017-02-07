#include "stdafx.h"

#include "NRenderLogic.h"

#include "Engine\Graphics\Effects\nrl\Logic\NRenderLogicImpl.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogic::~NRenderLogic                     ()
{
}

// **************************
//
NRenderLogic *           NRenderLogic::Create   ( NRenderLogicDesc & desc )
{
    return NRenderLogicImpl::Create( desc );
}

} //nrl
} //bv
