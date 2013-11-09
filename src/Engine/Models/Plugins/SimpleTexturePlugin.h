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
public:
    enum class MappingMode
    {
        MM_ATTACHED,
        MM_FREE,
    };

private:
    MappingMode     m_mappingMode;
    const IPlugin * m_prev;
    unsigned int    m_texCoordChannelIndex;

private:

    TextureInfo*        LoadTexture( const std::string& name, const std::string& path )   const;

    void                EvalGeometryChannel( const IPlugin* prev );

public:

    explicit                            SimpleTexturePlugin         ( const IPlugin* prev, const std::vector< std::string > & texturesFilesNames, MappingMode mappingType = MappingMode::MM_ATTACHED  );
                                        ~SimpleTexturePlugin        ();

    virtual const ITransformChannel*    GetTransformChannel         () const;

    void                                SetMappingMode              ( MappingMode mm );

    void                                Update                      ( float t ) override;
    void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
