#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

#include "Engine/Models/Plugins/PluginEnums.h"


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

    const IPlugin *         m_prev;

    TextureAttachmentMode   m_attachmentMode;
    unsigned int            m_texCoordChannelIndex;

private:

    TextureInfo *       LoadTexture( const std::string & name, const std::string & path )   const;

    void                EvalGeometryChannel( const IPlugin* prev );

public:

    explicit                            SimpleTexturePlugin         ( const IPlugin * prev, const std::vector< std::string > & texturesFilesNames, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                        ~SimpleTexturePlugin        ();

    virtual const ITransformChannel*    GetTransformChannel         () const;

    void                                SetAttachmentMode           ( TextureAttachmentMode mode );

    void                                Update                      ( float t ) override;
    void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
