#pragma once

#include "Engine/Models/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Mathematics\Transform\MatTransform.h"

namespace bv { namespace model {


// ***************************** DESCRIPTOR **********************************
class SimpleTexturePluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    static const std::string            alphaParamName;
    static const std::string            txMatrix0ParamName;
    static const std::string            txMatrix1ParamName;

    explicit SimpleTexturePluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleTexturePlugin : public BasePlugin< IPlugin, SimpleTexturePluginPD >
{
private:

    std::string         m_textureFileName;
    std::string         m_textureFileName1;

    ResourceHandle*     LoadTexture( const std::string& name, const std::string& path )   const;

public:

    explicit            SimpleTexturePlugin    ( const std::string & textureFileName, const std::string & textureFileName1 );
                        ~SimpleTexturePlugin   ();

    ///virtual std::string GetShaderFile               () const override;

    void                Update                      ( float t ) override;
    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

//// ***************************** DESCRIPTOR **********************************
//class SimpleTextureVertexPluginPD : public BaseParametersDescriptor
//{
//public:
//
//    static const std::string            pluginName;
//
//    explicit SimpleTextureVertexPluginPD();
//};
//
//
//// ***************************** PLUGIN ********************************** 
//class SimpleTextureVertexPlugin : public BasePlugin< IPlugin, SimpleTexturePixelPluginPD >
//{
//public:
//
//    explicit            SimpleTextureVertexPlugin   ();
//    virtual std::string GetShaderFile               () const override;
//
//    void                Update                      ( float t ) override;
//    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;
//
//};

} // model
} // bv
