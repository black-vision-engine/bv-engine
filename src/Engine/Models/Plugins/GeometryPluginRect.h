#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** UID **********************************
class GeometryRectPluginUID
{
public:

    static const char * GetName()   { return "geometry_rect_plugin"; }

};


// ***************************** PLUGIN ********************************** 
class GeometryRectPlugin : public BasePlugin< IPlugin, GeometryRectPluginUID >
{
private:

    model::VertexAttributesChannel *    m_geomChannel;

    ParamFloat                          m_width;
    ParamFloat                          m_height;

public:

    explicit            GeometryRectPlugin    ( const ParamFloat & w, const ParamFloat& h );
                        ~GeometryRectPlugin   ();

    virtual const IVertexAttributesChannel *    GetGeometryChannel  () const override;

    virtual void                                Update              ( TimeType t ) override;
    virtual void                                Print               ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
