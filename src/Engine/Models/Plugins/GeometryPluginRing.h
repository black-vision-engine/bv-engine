#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Mathematics\Transform\MatTransform.h"

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
public:

    explicit            GeometryRingPlugin    ();
                        ~GeometryRingPlugin   ();


    void                Update                      ( float t ) override;
    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv