#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model { 

class VertexAttributesChannel;

// ***************************** DESCRIPTOR **********************************
class GeometryPluginPD : public BaseParametersDescriptor
{
public:
    static const std::string            pluginName;

    explicit GeometryPluginPD();
};


// ***************************** PLUGIN ********************************** 
class GeometryPlugin : public BasePlugin< IPlugin, GeometryPluginPD >
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
