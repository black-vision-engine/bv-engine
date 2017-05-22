#include "stdafx.h"

#include "RenderLogic.h"

#include "Engine\Graphics\Effects\Logic\RenderLogicImpl.h"


namespace bv { 

// **************************
//
RenderLogic::~RenderLogic                     ()
{
}

// **************************
//
RenderLogic *           RenderLogic::Create   ( RenderLogicDesc & desc )
{
    return RenderLogicImpl::Create( desc );
}

} //bv
