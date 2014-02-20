#pragma once

#include <memory>

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

class SimpleVertexShaderChannel;

typedef std::shared_ptr< SimpleVertexShaderChannel >    SimpleVertexShaderChannelPtr;

// ***************************** UID **********************************
class SimpleVertexShaderPluginUID
{
public:
    static const char*       GetName()        { return "simple_vertex_shader_plugin"; }
};


// ***************************** PLUGIN ********************************** 
class SimpleVertexShaderPlugin : public BasePlugin< IPlugin >
{
private:

    SimpleVertexShaderChannelPtr    m_vshaderChannel;
    const std::string               m_shaderPath;

public:

    explicit                                SimpleVertexShaderPlugin    ( const IPlugin * prev, const std::string& shaderPath );
                                            ~SimpleVertexShaderPlugin   ();

    virtual const IVertexShaderChannel *    GetVertexShaderChannel      () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv