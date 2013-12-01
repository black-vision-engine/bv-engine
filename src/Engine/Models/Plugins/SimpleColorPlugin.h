#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"

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

    explicit                                SimpleColorPlugin           ( const IPlugin * prev, const ParamVec4 & color );
                                            ~SimpleColorPlugin          ();

    virtual const IPixelShaderChannel*      GetPixelShaderChannel       () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv