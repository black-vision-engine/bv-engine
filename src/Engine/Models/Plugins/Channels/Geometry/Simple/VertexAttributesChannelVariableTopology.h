#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"


namespace bv { namespace model {

class VariableTopologyStripComponent;

class VertexAttributesChannelVariableTopology : public VertexAttributesChannel
{
private:

    std::vector< VariableTopologyStripComponent  * >    m_vtConnectedComponents;

    float       m_size;
    float       m_speed;

    int         m_numSegments;
    int         m_numComponents;

    bool        m_needMoreUpdates;
    TimeType    m_curComponentStartTime;
    int         m_numActiveComponents;
    int         m_curComponent;

private:

                            VertexAttributesChannelVariableTopology     ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents );

public:

    virtual                 ~VertexAttributesChannelVariableTopology    ();

    virtual void            Update                              ( TimeType t );

    virtual bool            IsTimeInvariant                     ()              const;
    virtual bool            NeedsAttributesUpdate               ( TimeType t )  const;
    virtual bool            NeedsTopologyUpdate                 ( TimeType t )  const;

    virtual bool            CanBeConnectedTo                    ( IVertexAttributesChannel * channel ) const;

    void                    AddVTConnectedComponent             ( VariableTopologyStripComponent * cc );
    virtual unsigned int    TotalNumVertices                    () const;

    virtual std::vector< IConnectedComponent * >  GetComponents () const;

    static VertexAttributesChannelVariableTopology *    Create  ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents );

};

} //model
} //bv
