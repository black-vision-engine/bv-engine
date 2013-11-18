#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class SolidColorShaderChannel;
class ParamVec4;

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
private:

    SolidColorShaderChannel *   m_pixelShaderChannel;
    const IPlugin *             m_prev;

public:

    explicit                                SolidColorPlugin            ( const IPlugin * prev, const ParamVec4& color );
                                            ~SolidColorPlugin           ();

    virtual const IPixelShaderChannel *     GetPixelShaderChannel       () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
