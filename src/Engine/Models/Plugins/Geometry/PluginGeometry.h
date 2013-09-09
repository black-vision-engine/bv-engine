#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "Engine\Models\Plugin.h"

namespace bv {

class PluginGeometry : public BasePlugin<IGeometryGenPlugin>
{
protected:

    TVertexVec  m_vertices;
    TIndexVec   m_indices;

    Role        m_role;

public:

    const TVertexVec &          Vertices            () const;
    const TIndexVec &           Indices             () const;

    int                         VertexSize          () const;

    Role                        GetRole             () const;
    void                        SetRole             ( Role role );

    virtual void                Print               (std::ostream& out, int tabs = 0) const;

    explicit PluginGeometry(const std::string& name);
    virtual ~PluginGeometry(){};
};

}
