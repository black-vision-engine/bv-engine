#pragma once

#include "Engine\Models\Plugins\Channels\Geometry\GeometryChannel.h"

namespace bv { namespace model {

class VariableTopologyStripComponent;

class GeometryChannelVariableTopology : public GeometryChannel
{
private:

    std::vector< VariableTopologyStripComponent  * >    m_vtConnectedComponents;

    float   m_size;
    float   m_speed;

    int     m_numSegments;
    int     m_numComponents;

    bool    m_needMoreUpdates;
    float   m_curComponentStartTime;
    int     m_numActiveComponents;
    int     m_curComponent;

private:

                            GeometryChannelVariableTopology     ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents );

public:

    virtual                 ~GeometryChannelVariableTopology    ();

    virtual void            Update                              ( float t );

    virtual bool            IsTimeInvariant                     ()          const;
    virtual bool            NeedsAttributesUpdate                ( float t ) const;
    virtual bool            NeedsTopologyUpdate                 ( float t ) const;

    virtual bool            CanBeConnectedTo                    ( IGeometryChannel * channel ) const;

    void                    AddVTConnectedComponent             ( VariableTopologyStripComponent * cc );
    virtual unsigned int    TotalNumVertices                    () const;

    virtual std::vector< IConnectedComponent * >  GetComponents () const;

    static GeometryChannelVariableTopology *    Create  ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents );

};

} //model
} //bv
