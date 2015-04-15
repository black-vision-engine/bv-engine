#pragma once

#include "CoreDEF.h"

#include "Engine/Graphics/State/RendererStateEnums.h"


namespace bv {

class FillState
{
public:

    FillStateMode   fillMode;   // FSM_POLYGONS

public:

    FillState  ();

};

DEFINE_PTR_TYPE(FillState)
DEFINE_CONST_PTR_TYPE(FillState)

int loc_test();

} //bv
