#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Mathematics/Transform/MatTransform.h"

namespace bv { namespace model { 

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
public:

    explicit            GeometryPlugin  ();
                        ~GeometryPlugin ();

    void                Update          ( float t ) override;
    void                Print           ( std::ostream & out, int tabs = 0 ) const override;

};

}
}
