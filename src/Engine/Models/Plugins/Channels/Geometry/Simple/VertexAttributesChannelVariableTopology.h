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

    virtual void            Update                              ( TimeType t )  override;

    virtual bool            IsTimeInvariant                     ()  const  override;
    virtual bool            NeedsAttributesUpdate               ()  const  override;
    virtual bool            NeedsTopologyUpdate                 ()  const  override;

    virtual bool            CanBeConnectedTo                    ( IVertexAttributesChannelPtr channel ) const  override;

    void                    AddVTConnectedComponent             ( VariableTopologyStripComponent * cc );
    virtual unsigned int    TotalNumVertices                    ()  const  override;

    virtual std::vector< IConnectedComponent * >  GetComponents ()  const  override;

    static VertexAttributesChannelVariableTopology *    Create  ( float size, float speed, float oscilationSpeed, int numSegments, int numComponents );

};

} //model
} //bv
