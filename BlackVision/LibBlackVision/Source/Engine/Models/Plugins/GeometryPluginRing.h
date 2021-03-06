#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {


// ***************************** UID **********************************
class GeometryRingPluginUID
{
public:
    static const char*       GetName()        { return "geomtry_ring_plugin"; }
};


// ***************************** PLUGIN ********************************** 
class GeometryRingPlugin : public BasePlugin
{
private:

    model::VertexAttributesChannelPtr m_vaChannel;

public:

    explicit            GeometryRingPlugin    ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum );
                        ~GeometryRingPlugin   ();

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel          () const override;

    virtual void                        Update                      ( TimeType t ) override;
    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv
