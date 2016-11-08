#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** UID **********************************
class GeometryMultiRectPluginUID
{
public:

    static const char *       GetName()       { return "geometry_multi_rect_plugin"; }
};


// ***************************** PLUGIN ********************************** 
class GeometryMultiRectPlugin : public BasePlugin
{
private:

    model::VertexAttributesChannelPtr    m_vaChannel;

public:

    explicit            GeometryMultiRectPlugin    ();
                        ~GeometryMultiRectPlugin   ();

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel          () const override;
    void                                Update                      ( TimeType t ) override;
    void                                Print                       ( std::ostream & out, int tabs = 0 ) const;

    void                                AddRectConnectedComponnent  ( const ParamFloat& w, const ParamFloat& h, const ParamFloat& tx, const ParamFloat& ty, const ParamFloat& tz );
    void                                AddRectConnectedComponnent  ( float w, float h, float tx = 0.f, float ty = 0.f, float tz = 0.f );

};

} // model
} // bv
