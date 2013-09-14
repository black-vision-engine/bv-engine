
#include "PluginTransform.h"
#include <cassert>

namespace bv {

// *********************************
//
void PrintMatrixDebugOnlyLocal        ( const glm::mat4 mat, std::ostream & out, int tabs )
{
    for(int row = 0; row < 4; ++row)
    {
        for(int col = 0; col < 4; ++col)
        {
            out << mat[col][row] << " ";
        }
        out << debug::EndLine(tabs);
    }
}

}
