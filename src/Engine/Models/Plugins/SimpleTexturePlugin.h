#pragma once

#include "Engine/Models/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv {

// ***************************** DESCRIPTOR **********************************
class TexturePluginParametersDescriptor : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit TexturePluginParametersDescriptor();
};


// ***************************** PLUGIN ********************************** 
class SimpleTexturePlugin : public BasePlugin< IShaderPlugin, TexturePluginParametersDescriptor >
{
private:

    std::string     m_textureFileName;

public:

    explicit            SimpleTexturePlugin ( const std::string & textureFileName );
    virtual std::string GetShaderFile       () const override;


    void                Update              ( float t ) override;
    void                Print               ( std::ostream & out, int tabs = 0 ) const override;
};

} // bv