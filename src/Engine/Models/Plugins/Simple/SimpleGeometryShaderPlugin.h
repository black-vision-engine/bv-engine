#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

class SimpleGeometryShaderChannel;

// ***************************** UID **********************************
class SimpleGeometryShaderPluginUID
{
public:
    static const char*       GetName()        { return "simple_geometry_shader_plugin"; }
};


// ***************************** PLUGIN ********************************** 
class SimpleGeometryShaderPlugin : public BasePlugin< IPlugin, SimpleGeometryShaderPluginUID >
{
private:

    SimpleGeometryShaderChannel*    m_gshaderChannel;
    const std::string               m_shaderPath;

public:

    explicit                                SimpleGeometryShaderPlugin  ( const IPlugin * prev, const std::string& shaderPath );
                                            ~SimpleGeometryShaderPlugin ();

    virtual const IGeometryShaderChannel*   GetGeometryShaderChannel    () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv