#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class SolidColorShaderChannel;

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

    SolidColorShaderChannel* m_pixelShaderChannel;
    const IPlugin * m_prev;

public:

    explicit                                SolidColorPlugin            ( const IPlugin * prev, const Vec4Interpolator& color );
                                            ~SolidColorPlugin           ();

    virtual const IPixelShaderChannel*      GetPixelShaderChannel       () const;

    void                                    Update                      ( TimeType t ) override;
    void                                    Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv