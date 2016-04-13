#pragma once

#include "TabStop.h"
#include "Engine/Models/BVProjectEditor.h"

#include <vector>

namespace bv
{



class TabStopsLogic
{
private:

    std::vector< TabStopPtr >       m_tabStops;

public:
    TabStopsLogic();
    ~TabStopsLogic();
};


}	// bv
