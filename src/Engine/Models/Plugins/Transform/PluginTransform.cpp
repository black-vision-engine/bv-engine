
#include "PluginTransform.h"
#include <cassert>

#include "System/Print.h"
#include "Engine\Models\Parameter.h"

namespace bv {

using namespace debug;
// *********************************
//
PluginTransform::PluginTransform(const std::string& name)
    : BasePlugin<ITransformPlugin>(name)
    , m_value(nullptr)
{

}

// *********************************
//
const glm::mat4 &           PluginTransform::TransformMatrix     () const
{
    return m_value->GetValue();
}

namespace
{

void PrintMatrix(const glm::mat4 mat, std::ostream& out, int tabs)
{
    for(int row = 0; row < 4; ++row)
    {
        for(int col = 0; col < 4; ++col)
        {
            out << mat[col][row] << " ";
        }
        out << EndLine(tabs);
    }
}

}

void PluginTransform::Print(std::ostream& out, int tabs) const
{
    out << m_pluginName << EndLine(tabs);
    out << "PARAMS:" << EndLine(tabs);
    out << "Transform:" << EndLine(tabs);
    PrintMatrix(m_value->GetValue(), out, tabs);
}

}
