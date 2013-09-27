#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Mathematics\Transform\MatTransform.h"

namespace bv { namespace model {

class Resource;

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
    typedef std::pair< ResourceHandle*, std::string > TexturePair;


    std::string         m_textureFileName;
    std::string         m_textureFileName1;


    TexturePair         LoadTexture( const std::string& name, const std::string& path )   const;

    void                EvalGeometryChannel( const IPlugin* prev );

public:

    explicit            SimpleTexturePlugin    ( const IPlugin* prev, const std::string & textureFileName, const std::string & textureFileName1 );
                        ~SimpleTexturePlugin   ();


    void                Update                      ( float t ) override;
    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
