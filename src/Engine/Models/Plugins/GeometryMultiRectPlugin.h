#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class ParamFloat;

// ***************************** DESCRIPTOR **********************************
class GeometryMultiRectPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit GeometryMultiRectPluginPD();

};


// ***************************** PLUGIN ********************************** 
class GeometryMultiRectPlugin : public BasePlugin< IPlugin, GeometryMultiRectPluginPD >
{
private:

    model::GeometryChannel*     m_geomChannel;

public:

    explicit            GeometryMultiRectPlugin    ();
                        ~GeometryMultiRectPlugin   ();

    virtual const IGeometryChannel *    GetGeometryChannel          () const override;
    void                                Update                      ( TimeType t ) override;
    void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

    void                                AddRectConnectedComponnent  ( const ParamFloat& w, const ParamFloat& h, const ParamFloat& tx, const ParamFloat& ty, const ParamFloat& tz );
    void                                AddRectConnectedComponnent  ( float w, float h, float tx = 0.f, float ty = 0.f, float tz = 0.f );

};

} // model
} // bv
