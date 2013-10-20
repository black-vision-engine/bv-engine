#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv { namespace model {


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

    const IPlugin * m_prev;

public:

    explicit                                SolidColorPlugin            ( const IPlugin * prev );
                                            ~SolidColorPlugin           ();

    virtual const IGeometryChannel*         GetGeometryChannel          () const;
    virtual const ITransformChannel*        GetTransformChannel         () const;
    //virtual const IPixelShaderChannel*      GetPixelShaderChannel       () const;
    //virtual const IVertexShaderChannel*     GetVertexShaderChannel      () const;
    //virtual const IGeometryShaderChannel*   GetGeometryShaderChannel    () const;


    void                                    Update                      ( float t ) override;
    void                                    Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv