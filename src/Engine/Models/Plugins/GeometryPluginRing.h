#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {


// ***************************** DESCRIPTOR **********************************
class GeometryRingPluginPD : public BaseParametersDescriptor
{
public:
    static const std::string            pluginName;

    explicit GeometryRingPluginPD();
};


// ***************************** PLUGIN ********************************** 
class GeometryRingPlugin : public BasePlugin< IPlugin, GeometryRingPluginPD >
{
    model::GeometryChannel* m_geomChannel;
public:

    explicit            GeometryRingPlugin    ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum );
                        ~GeometryRingPlugin   ();

    virtual const IGeometryChannel *    GetGeometryChannel          () const override;

    void                                Update                      ( float t ) override;
    void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv