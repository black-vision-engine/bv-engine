#pragma once

#include "Engine/Models/Plugins/Plugin.h"

namespace bv { namespace model {

class SimpleColorPixelShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleColorPluginUID
{
public:

    static const char * GetName()   { return "simple_color_plugin"; }

};


// ***************************** PLUGIN ********************************** 
class SimpleColorPlugin : public BasePlugin< IPlugin >
{
private:

    SimpleColorPixelShaderChannel *         m_pshaderChannel;

public:

    explicit                                SimpleColorPlugin           ( IPluginConstPtr prev, const ParamVec4 & color );
                                            ~SimpleColorPlugin          ();

    virtual const IPixelShaderChannel *     GetPixelShaderChannel       () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv
