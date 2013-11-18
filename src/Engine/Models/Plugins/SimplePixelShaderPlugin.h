#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class SimplePixelShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimplePixelShaderPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimplePixelShaderPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimplePixelShaderPlugin : public BasePlugin< IPlugin, SimplePixelShaderPluginPD >
{
private:

    SimplePixelShaderChannel*   m_pshaderChannel;
    const std::string           m_shaderPath;

public:

    explicit                                SimplePixelShaderPlugin     ( const IPlugin * prev, const std::string& shaderPath, RendererContext * ctx = nullptr );
                                            ~SimplePixelShaderPlugin    ();

    virtual const IPixelShaderChannel*      GetPixelShaderChannel       () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv