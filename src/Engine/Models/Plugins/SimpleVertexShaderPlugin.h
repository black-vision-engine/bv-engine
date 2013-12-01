#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"


namespace bv { namespace model {

class SimpleVertexShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleVertexShaderPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleVertexShaderPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleVertexShaderPlugin : public BasePlugin< IPlugin, SimpleVertexShaderPluginPD >
{
private:

    SimpleVertexShaderChannel*      m_vshaderChannel;
    const std::string               m_shaderPath;

public:

    explicit                                SimpleVertexShaderPlugin    ( const IPlugin * prev, const std::string& shaderPath );
                                            ~SimpleVertexShaderPlugin   ();

    virtual const IVertexShaderChannel *    GetVertexShaderChannel      () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv