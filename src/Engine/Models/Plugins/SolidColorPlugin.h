#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv { namespace model {


// ***************************** DESCRIPTOR **********************************
class SolidColorPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SolidColorPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SolidColorPlugin : public BasePlugin< IPlugin, SolidColorPluginPD >
{
public:

    explicit            SolidColorPlugin    ( const IPlugin* prev );
                        ~SolidColorPlugin   ();


    void                Update                      ( float t ) override;
    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv