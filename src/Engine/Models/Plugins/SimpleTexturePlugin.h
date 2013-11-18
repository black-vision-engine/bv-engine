#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

#include "Engine/Models/Plugins/PluginEnums.h"


namespace bv { namespace model {

class Resource;
class TexturePixelShaderChannel;
class TextureVertexShaderChannel;

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
    TextureAttachmentMode   m_attachmentMode;
    unsigned int            m_texCoordChannelIndex;
    GeometryChannel*        m_geomChannel;
    TexturePixelShaderChannel*  m_pixelShaderChannel;
    TextureVertexShaderChannel* m_vertexShaderChannel;

private:

    TextureInfo *       LoadTexture( const std::string & name, const std::string & path )   const;

    void                EvalGeometryChannel( const IPlugin* prev );

public:

    explicit                            SimpleTexturePlugin         ( const IPlugin * prev, const std::vector< std::string > & texturesFilesNames, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                        ~SimpleTexturePlugin        ();

    virtual const IGeometryChannel*     GetGeometryChannel          () const override;                                                                           
    virtual const IPixelShaderChannel*  GetPixelShaderChannel       () const override;                                       
    virtual const IVertexShaderChannel* GetVertexShaderChannel      () const override;       

    void                                SetAttachmentMode           ( TextureAttachmentMode mode );

    virtual void                        Update                      ( TimeType t ) override;
    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
