#include "PluginGeometry.h"
#include <cassert>

#include "System/Print.h"

namespace bv {

using namespace debug;

// *********************************
//
PluginGeometry::PluginGeometry(const std::string& name)
    : BasePlugin<IGeometryGenPlugin>(name)
{

}

// *********************************
//
const PluginGeometry::TVertexVec &          PluginGeometry::Vertices    () const //TODO: dodac czas
{
    return m_vertices;
}

// *********************************
//
const PluginGeometry::TIndexVec &           PluginGeometry::Indices     () const //TODO: dodac czas
{
    return m_indices;

}

// *********************************
//
int                                         PluginGeometry::VertexSize   () const 
{
    return sizeof(glm::vec3);
}

// *********************************
//
PluginGeometry::Role  PluginGeometry::GetRole             () const
{
    return m_role;
}

// *********************************
//
void PluginGeometry::SetRole             ( PluginGeometry::Role role )
{
    m_role = role;
}



void PluginGeometry::Print(std::ostream& out, int tabs) const
{
    out << "Vertices :" << EndLine(tabs);
    for(auto v : m_vertices)
    {
        out << "(" << v.x << v.y << v.z << ")" << EndLine(tabs);
    }

    out << EndLine(tabs);
    
    out << "Indices :" << EndLine(tabs);
    for(auto i : m_indices)
    {
        out << i << " ";
    }

    out << EndLine(tabs);
}

}
