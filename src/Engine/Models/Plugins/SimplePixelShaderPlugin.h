#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv { namespace model {


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

    const IPlugin *     m_prev;
    const std::string   m_shaderPath;

public:

    explicit                                SimplePixelShaderPlugin     ( const IPlugin * prev, const std::string& shaderPath );
                                            ~SimplePixelShaderPlugin    ();

    virtual const IGeometryChannel*         GetGeometryChannel          () const;
    virtual const ITransformChannel*        GetTransformChannel         () const;
    virtual const IPixelShaderChannel*      GetPixelShaderChannel       () const;
    virtual const IVertexShaderChannel*     GetVertexShaderChannel      () const;
    virtual const IGeometryShaderChannel*   GetGeometryShaderChannel    () const;

    void                                    Update                      ( float t ) override;
    void                                    Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv