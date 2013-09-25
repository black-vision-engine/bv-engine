#pragma once

#include "Engine/Models/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv {

// ***************************** DESCRIPTOR **********************************
class SimpleTexturePixelPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    static const std::string            alfaParamName;

    explicit SimpleTexturePixelPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleTexturePixelPlugin : public BasePlugin< IShaderPlugin, SimpleTexturePixelPluginPD >
{
private:

    std::string     m_textureFileName;

public:

    explicit            SimpleTexturePixelPlugin    ( const std::string & textureFileName );
    virtual std::string GetShaderFile               () const override;

    void                Update                      ( float t ) override;
    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

// ***************************** DESCRIPTOR **********************************
class SimpleTextureVertexPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleTextureVertexPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleTextureVertexPlugin : public BasePlugin< IShaderPlugin, SimpleTexturePixelPluginPD >
{
public:

    explicit            SimpleTextureVertexPlugin   ();
    virtual std::string GetShaderFile               () const override;

    void                Update                      ( float t ) override;
    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // bv
