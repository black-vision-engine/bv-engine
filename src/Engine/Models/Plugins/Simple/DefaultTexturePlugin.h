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

// ***************************** DefaultTexturePlugin TextureDescriptor ***

class TextureDescriptor
{
public:

    std::string             fileName;
    ParamTransform          transform;

    ParamFloat              alpha;
    ParamVec4               borderColor;

    TextureWrappingMode     wrappingModeX;
    TextureWrappingMode     wrappingModeY;
    TextureFilteringMode    filteringMode;

    explicit TextureDescriptor  (    const std::string & txFileName
                                ,    const ParamTransform & txTransform = ParametersFactory::CreateParameter( "texTransform", TransformF() )
                                ,    const ParamFloat& txAlpha = ParametersFactory::CreateParameter( "texAlpha", InterpolatorsHelper::CreateConstValue( 1.f ) )
                                ,    const ParamVec4& txBorderColor = ParametersFactory::CreateParameter( "texBorderColor", InterpolatorsHelper::CreateConstValue( glm::vec4( 0.f, 0.f, 0.f, 0.f ) ) )
                                ,    TextureWrappingMode txWrappingModeX = TextureWrappingMode::TWM_REPEAT 
                                ,    TextureWrappingMode txWrappingModeY = TextureWrappingMode::TWM_REPEAT
                                ,    TextureFilteringMode txFilteringMode = TextureFilteringMode::TFM_LINEAR
                                )
                                : fileName( txFileName )
                                , transform( txTransform )
                                , alpha( txAlpha )
                                , borderColor( txBorderColor )
                                , wrappingModeX( txWrappingModeX )
                                , wrappingModeY( txWrappingModeY )
                                , filteringMode( txFilteringMode )
    {}
};

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
