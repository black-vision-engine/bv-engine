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

    explicit SimpleTexturePluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleTexturePlugin : public BasePlugin< IPlugin, SimpleTexturePluginPD >
{
private:

    typedef std::pair< ResourceHandle*, std::string > TexturePair;

private:

    const IPlugin * m_prev;

private:

    TexturePair         LoadTexture( const std::string& name, const std::string& path )   const;

    void                EvalGeometryChannel( const IPlugin* prev );

public:

    explicit                            SimpleTexturePlugin         ( const IPlugin* prev, const std::vector< std::string > & texturesFilesNames  );
                                        ~SimpleTexturePlugin        ();

    virtual const ITransformChannel*    GetTransformChannel         () const;

    void                                Update                      ( float t ) override;
    void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
