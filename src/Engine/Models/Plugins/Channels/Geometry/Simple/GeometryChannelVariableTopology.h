#pragma once

#include "Engine\Models\Plugins\Channels\Geometry\GeometryChannel.h"

namespace bv { namespace model {

class VariableTopologyStripComponent;

class GeometryChannelVariableTopology : public GeometryChannel
{
private:

    std::vector< VariableTopologyStripComponent  * >    m_allConnectedComponents;

    float   m_size;
    float   m_speed;

    int     m_numSegments;
    int     m_numComponents;

    int     m_numActiveComponents;
    int     m_curComponent;

private:

                    GeometryChannelVariableTopology     ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents );

public:

    virtual         ~GeometryChannelVariableTopology    ();

    virtual void    Update                              ( float t );

    virtual bool    NeedsPositionsUpdate                ( float t ) const;
    virtual bool    NeedsTopologyUpdate                 ( float t ) const;

    static GeometryChannelVariableTopology *    Create  ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents );

};

} //model
} //bv
