#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"

namespace bv { namespace model {

class SimpleColorPixelShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleColorPluginUID
{
public:
    static const char*       GetName()        { return "simple_color_plugin"; }
};


// ***************************** PLUGIN ********************************** 
class SimpleColorPlugin : public BasePlugin< IPlugin, SimpleColorPluginUID >
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