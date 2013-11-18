#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {


// ***************************** DESCRIPTOR **********************************
class GeometryRectPluginPD : public BaseParametersDescriptor
{
public:
    static const std::string            pluginName;

    explicit GeometryRectPluginPD();
};


// ***************************** PLUGIN ********************************** 
class GeometryRectPlugin : public BasePlugin< IPlugin, GeometryRectPluginPD >
{
private:

    model::GeometryChannel *    m_geomChannel;
    FloatInterpolator           m_width;
    FloatInterpolator           m_height;

public:
    explicit            GeometryRectPlugin    ( const FloatInterpolator& w, const FloatInterpolator& h );
                        ~GeometryRectPlugin   ();

    virtual const IGeometryChannel *    GetGeometryChannel          () const override;
    virtual void                        Update                      ( TimeType t ) override;
    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
