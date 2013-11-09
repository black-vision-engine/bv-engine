#pragma once

namespace bv { namespace model {

class FillContext
{
public:

    enum class Mode : int
    {
        M_POINTS = 0,
        M_LINES,
        M_POLYGONS,

        M_TOTAL
    };

public:

    Mode fillMode;

public:

    FillContext();

};

} //model
} //bv
