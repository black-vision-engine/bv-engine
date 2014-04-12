#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultTexturePluginDesc : public BasePluginDescriptor
{
public:

    DefaultTexturePluginDesc                                    ();

    virtual IPlugin *                       CreatePlugin        ( const std::string & name, const IPlugin * prev ) const override;
    virtual DefaultPluginParamValModel *    CreateDefaultModel  () const override;
   
    static  std::string                     UID                 ();

    static  std::string                     VertexShaderSource  ();
    static  std::string                     PixelShaderSource   ();

};

class Resource;
class TexturePixelShaderChannel;
class TextureVertexShaderChannel;


// ***************************** PLUGIN ********************************** 
class DefaultTexturePlugin : public BasePlugin< IPlugin >
{
private:

    TextureAttachmentMode           m_attachmentMode;

    unsigned int                    m_texCoordChannelIndex;

    DefaultPixelShaderChannelPtr    m_psc;

    VertexAttributesChannelPtr      m_vaChannel;
    TexturePixelShaderChannelPtr    m_pixelShaderChannel;
    TextureVertexShaderChannelPtr   m_vertexShaderChannel;

    TextureInfoVec                  m_textures;

public:

    bool                            SetTexture( const std::string & textureFile, const std::string & name );

private:

    TextureInfo *                               LoadTexture( const TextureDescriptor & texDesc, const std::string & name )   const;
    void                                        EvalGeometryChannel( const IPlugin * prev );

public:

    explicit                                    DefaultTexturePlugin         ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model )
        : BasePlugin< IPlugin >(  name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    {
    }


    virtual bool                                LoadResource                ( const IPluginResourceDescr * resDescr ) override;
    explicit                                    DefaultTexturePlugin         ( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, TextureAttachmentMode amode = TextureAttachmentMode::MM_ATTACHED );
    explicit                                    DefaultTexturePlugin         ( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, model::RendererContext * ctx = nullptr, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                                ~DefaultTexturePlugin        ();

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;                                                                           
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;                                       
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;     

    virtual TextureInfoVec                      GetTextures                 () const override;

    void                                        SetAttachmentMode           ( TextureAttachmentMode mode );
    void                                        SetWrappingMode             ( TextureWrappingMode mode );

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv
