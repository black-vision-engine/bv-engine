#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"


namespace bv { namespace model { 

class VertexAttributesChannel;

// ***************************** UID **********************************
class GeometryPluginUID
{
public:
    static const char*       GetName()        { return "geometry_plugin"; }
};


// ***************************** PLUGIN ********************************** 
class GeometryPlugin : public BasePlugin< IPlugin, GeometryPluginUID >
{
private:

    VertexAttributesChannel *   m_geomChannel;

public:

    explicit            GeometryPlugin  ( model::VertexAttributesChannel * geomChannel );
                        ~GeometryPlugin ();

    const IVertexAttributesChannel *    GetGeometryChannel  () const override;

    virtual void                Update              ( TimeType t ) override;
    virtual void                Print               ( std::ostream & out, int tabs = 0 ) const override;

};

} //model
} //bv
