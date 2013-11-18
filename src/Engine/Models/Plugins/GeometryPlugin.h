#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model { 

class GeometryChannel;

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

    GeometryChannel *   m_geomChannel;

public:

    explicit            GeometryPlugin  ( model::GeometryChannel * geomChannel );
                        ~GeometryPlugin ();

    const IGeometryChannel *    GetGeometryChannel  () const override;

    virtual void                Update              ( TimeType t ) override;
    virtual void                Print               ( std::ostream & out, int tabs = 0 ) const override;

};

} //model
} //bv
