#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {


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

    const IPlugin*          m_prev;
    const std::string       m_shaderPath;

public:

    explicit                                SimpleVertexShaderPlugin    ( const IPlugin * prev, const std::string& shaderPath );
                                            ~SimpleVertexShaderPlugin   ();

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