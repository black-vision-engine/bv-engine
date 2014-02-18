#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"


namespace bv { namespace model {

class SimpleColorPixelShaderChannel;
class DefaultColorPlugin;

// ***************************** DESCRIPTOR **********************************
class SimpleColorPluginDesc
{
public:

    static const char *                 GetName     ()   { return "default_color_plugin"; }

    static DefaultPluginParamValModel * CreateModel ( bool setDefaultValues );
    static DefaultColorPlugin *         CreatePlugin( const IPlugin * prev, bool setDefaultValues = true );

};


// ***************************** PLUGIN ********************************** 
class DefaultColorPlugin : public BasePlugin< IPlugin, SimpleColorPluginDesc >
{
private:

    SimpleColorPixelShaderChannel *         m_pshaderChannel;

public:

    explicit                                DefaultColorPlugin          ( const IPlugin * prev, const ParamVec4 & color );
                                            ~DefaultColorPlugin         ();

    virtual const IPixelShaderChannel *     GetPixelShaderChannel       () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv
