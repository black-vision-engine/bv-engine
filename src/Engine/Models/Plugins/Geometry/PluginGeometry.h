#pragma once

#include <vector>
#include "System/Print.h"

#include "glm/glm.hpp"

#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugin.h"

//FIXME: REMOVE FROM PROJECT
//FIXME: when required functionality is copied to channels
namespace bv {

template< typename PluginGeometryParametersDescriptor >
class PluginGeometry : public BasePlugin<IGeometryGenPlugin, PluginGeometryParametersDescriptor>
{
private:

    typedef BasePlugin<IGeometryGenPlugin, PluginGeometryParametersDescriptor> ParentClass;

protected:

    IGeometryGenPlugin::TVertexVec  m_vertices;
    IGeometryGenPlugin::TIndexVec   m_indices;

    IGeometryGenPlugin::Role        m_role;

public:

    const IGeometryGenPlugin::TVertexVec & Vertices            () const { return m_vertices; }
    const IGeometryGenPlugin::TIndexVec &  Indices             () const { return m_indices; }

    int                             VertexSize          () const { return sizeof( glm::vec3 ); }

    IGeometryGenPlugin::Role        GetRole             () const { return m_role; }
    void                            SetRole             ( IGeometryGenPlugin::Role role ) { m_role = role; }

    virtual void                    Print               ( std::ostream& out, int tabs = 0 ) const;

    explicit                        PluginGeometry      () {}
    virtual                         ~PluginGeometry     () {}

};

template< typename PluginGeometryParametersDescriptor >
void PluginGeometry< PluginGeometryParametersDescriptor >::Print( std::ostream & out, int tabs ) const
{
    out << "Vertices :" << debug::EndLine(tabs);
    for(auto v : m_vertices)
    {
        out << "(" << v.x << v.y << v.z << ")" << debug::EndLine(tabs);
    }

    out << debug::EndLine(tabs);
    
    out << "Indices :" << debug::EndLine(tabs);
    for(auto i : m_indices)
    {
        out << i << " ";
    }

    out << debug::EndLine( tabs );
}

void PluginGeometryDummySymbol();

}
