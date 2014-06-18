#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"

namespace bv { namespace model {

// ***************************** UID **********************************
class SimplePixelShaderPluginUID
{
public:
    static const char*       GetName()        { return "simple_pixel_shader_plugin"; }
};

class SimplePixelShaderChannelPD
{
};


class SimplePixelShaderChannel : public DefaultPixelShaderChannel
{
public:

    explicit                        SimplePixelShaderChannel( const std::string & shaderFile, RendererContextPtr ctx = nullptr )
        : DefaultPixelShaderChannel( shaderFile, nullptr, ctx )
    {}

    virtual ITexturesDataConstPtr   GetTexturesData() const override
    {
        return nullptr;
    }

};

DEFINE_PTR_TYPE(SimplePixelShaderChannel)
DEFINE_CONST_PTR_TYPE(SimplePixelShaderChannel)

// ***************************** PLUGIN ********************************** 
class SimplePixelShaderPlugin : public BasePlugin< IPlugin >
{
private:

    SimplePixelShaderChannelPtr m_pshaderChannel;
    const std::string           m_shaderPath;

public:

    explicit                                SimplePixelShaderPlugin     ( IPluginConstPtr prev, const std::string & shaderPath, RendererContextPtr ctx = nullptr );
                                            ~SimplePixelShaderPlugin    ();

    virtual IPixelShaderChannelConstPtr     GetPixelShaderChannel       () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv