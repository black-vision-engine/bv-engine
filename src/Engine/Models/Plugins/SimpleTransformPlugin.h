#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class SimpleTransformChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleTransformPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleTransformPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleTransformPlugin : public BasePlugin< IPlugin, SimpleTransformPluginPD >
{
    SimpleTransformChannel*                 m_transformChannel;

public:

    explicit                                SimpleTransformPlugin       ( const IPlugin * prev, TransformF* trans );
                                            ~SimpleTransformPlugin      ();

    virtual const IGeometryChannel*         GetGeometryChannel          () const;
    virtual const ITransformChannel*        GetTransformChannel         () const;
    virtual const IPixelShaderChannel*      GetPixelShaderChannel       () const;
    virtual const IVertexShaderChannel*     GetVertexShaderChannel      () const;
    virtual const IGeometryShaderChannel*   GetGeometryShaderChannel    () const;

    void                                    Update                      ( float t ) override;
    void                                    Print                       ( std::ostream & out, int tabs = 0 ) const override;

    static  SimpleTransformPlugin*          Create                      ( const IPlugin * prev, TransformF* trans );
};

} // model
} // bv