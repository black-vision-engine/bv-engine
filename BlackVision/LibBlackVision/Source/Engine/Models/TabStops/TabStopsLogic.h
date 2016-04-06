#pragma once

#include "TabStop.h"
#include "Engine/Models/BVProjectEditor.h"

#include <vector>

namespace bv
{



class TabStopsLogic
{
private:

    BVProjectEditor                 m_editor;
    std::vector< TabStopPtr >       m_tabStops;

public:
    TabStopsLogic( BVProjectEditor editor );
    ~TabStopsLogic();
};


}	// bv
