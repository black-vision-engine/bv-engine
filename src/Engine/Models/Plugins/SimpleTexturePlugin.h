#pragma once

#include "Engine/Models/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Mathematics\Transform\MatTransform.h"

namespace bv {

class ParamFloat;
class ValueFloat;
class ParamTransform;
class ValueMat4;

// ***************************** DESCRIPTOR **********************************
class SimpleTexturePixelPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    static const std::string            alphaParamName;
    static const std::string            txMatrix0ParamName;
    static const std::string            txMatrix1ParamName;

    explicit SimpleTexturePixelPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleTexturePixelPlugin : public BasePlugin< IShaderPlugin, SimpleTexturePixelPluginPD >
{
private:

    std::string         m_textureFileName;
    std::string         m_textureFileName1;

    ParamFloat *        m_alphaParam;
    ValueFloat *        m_alphaValue;

    ParamTransform *    m_tex0TransformParam;
    ValueMat4 *         m_tex0TransformValue;

    ParamTransform *    m_tex1TransformParam;
    ValueMat4 *         m_tex1TransformValue;

public:

    explicit            SimpleTexturePixelPlugin    ( const std::string & textureFileName, const std::string & textureFileName1, const FloatInterpolator & alpha, const TransformF & tex0Transform, const TransformF & tex1Transform );
                        ~SimpleTexturePixelPlugin   ();

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
