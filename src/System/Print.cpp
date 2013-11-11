#include "Print.h"


namespace bv
{
namespace debug
{
// *******************************
//
std::string EndLine(int tabs)
{
    std::string out;
    out += "\n";
    for(int t = 0; t < tabs; ++t)
        out += "\t";

    return out;
}
} // debug
} // bv