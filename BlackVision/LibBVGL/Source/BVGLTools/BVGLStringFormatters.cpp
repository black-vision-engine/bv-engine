#include "BVGLStringFormatters.h"

#include <sstream>
#include <iomanip>


namespace bv {


// ****************************
//
std::string FormatSizeString( size_t numBytes )
{
    static const std::string units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

    auto idx = 0;
    auto rem = 0.f;

    while( numBytes > 1024 )
    {
        rem = float( numBytes % 1024 ) / 1024.f;
        numBytes /= 1024;

        ++idx;
    }

    auto val = float(numBytes) + rem;

    std::stringstream ss;
    ss << std::setprecision(3) << val << units[ idx ];

    return ss.str();
}

} // bv
