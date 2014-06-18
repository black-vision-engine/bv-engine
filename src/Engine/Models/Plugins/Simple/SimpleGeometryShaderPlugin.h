#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"

namespace bv { namespace model {

class SimpleGeometryShaderChannel;

// ***************************** UID **********************************
class SimpleGeometryShaderPluginUID
{
public:
    static const char*       GetName()        { return "simple_geometry_shader_plugin"; }
};


class SimpleGeometryShaderChannel : public ShaderChannel< model::IGeometryShaderChannel >
{
public:

    explicit                        SimpleGeometryShaderChannel( const std::string & shaderFile )
        : ShaderChannel( shaderFile, nullptr )
    {
    }

    virtual void                    Update( TimeType t )
    {
        //ShaderChannel::Update( t );
    }

};

DEFINE_PTR_TYPE(SimpleGeometryShaderChannel)


// ***************************** PLUGIN ********************************** 
class SimpleGeometryShaderPlugin : public BasePlugin< IPlugin >
{
private:

    SimpleGeometryShaderChannelPtr  m_gshaderChannel;
    const std::string               m_shaderPath;

public:

    explicit                                SimpleGeometryShaderPlugin  ( IPluginConstPtr prev, const std::string& shaderPath );
                                            ~SimpleGeometryShaderPlugin ();

    virtual IGeometryShaderChannelConstPtr  GetGeometryShaderChannel    () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv