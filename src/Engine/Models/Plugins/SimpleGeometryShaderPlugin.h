#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv { namespace model {


// ***************************** DESCRIPTOR **********************************
class SimpleGeometryShaderPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleGeometryShaderPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleGeometryShader : public BasePlugin< IPlugin, SimpleGeometryShaderPluginPD >
{
private:

    const IPlugin * m_prev;

public:

    explicit                                SimpleGeometryShader          ( const IPlugin * prev, const std::string& shaderPath );
                                            ~SimpleGeometryShader         ();

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