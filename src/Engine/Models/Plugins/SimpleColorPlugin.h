#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class SimpleColorPixelShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleColorPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleColorPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleColorPlugin : public BasePlugin< IPlugin, SimpleColorPluginPD >
{
private:
    SimpleColorPixelShaderChannel*      m_pshaderChannel;

public:

    explicit                                SimpleColorPlugin           ( const IPlugin * prev, const Vec4Interpolator& color );
                                            ~SimpleColorPlugin          ();

    virtual const IPixelShaderChannel*      GetPixelShaderChannel       () const;

    void                                    Update                      ( TimeType t ) override;
    void                                    Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv