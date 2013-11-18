#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class SimpleGeometryShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleGeometryShaderPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleGeometryShaderPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleGeometryShaderPlugin : public BasePlugin< IPlugin, SimpleGeometryShaderPluginPD >
{
private:

    SimpleGeometryShaderChannel*    m_gshaderChannel;
    const std::string               m_shaderPath;

public:

    explicit                                SimpleGeometryShaderPlugin  ( const IPlugin * prev, const std::string& shaderPath );
                                            ~SimpleGeometryShaderPlugin ();

    virtual const IGeometryShaderChannel*   GetGeometryShaderChannel    () const;

    void                                    Update                      ( TimeType t ) override;
    void                                    Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv