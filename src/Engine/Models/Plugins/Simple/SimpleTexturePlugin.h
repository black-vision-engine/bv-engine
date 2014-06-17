#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"

#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {

class Resource;
class TexturePixelShaderChannel;
class TextureVertexShaderChannel;

// ***************************** UID **********************************
class SimpleTexturePluginUID
{
public:

    static const char * GetName()   { return "simple_texture_shader_plugin"; }

};

// ***************************** SimpleTexturePlugin TextureDescriptor ***

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
                                ,    const ParamTransform & txTransform = ParametersFactory::CreateParameter( "texTransform", TransformF(), nullptr )
                                ,    const ParamFloat& txAlpha = ParametersFactory::CreateParameter( "texAlpha", InterpolatorsHelper::CreateConstValue( 1.f ), nullptr )
                                ,    const ParamVec4& txBorderColor = ParametersFactory::CreateParameter( "texBorderColor", InterpolatorsHelper::CreateConstValue( glm::vec4( 0.f, 0.f, 0.f, 0.f ) ), nullptr )
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
class SimpleTexturePlugin : public BasePlugin< IPlugin >
{
private:

    TextureAttachmentMode           m_attachmentMode;

    unsigned int                    m_texCoordChannelIndex;

    VertexAttributesChannelPtr      m_vaChannel;
    TexturePixelShaderChannelPtr    m_pixelShaderChannel;
    TextureVertexShaderChannelPtr   m_vertexShaderChannel;

    TextureInfoVec                  m_textures;

private:

    TextureInfo *                               LoadTexture( const TextureDescriptor & texDesc, const std::string & name )   const;
    void                                        EvalGeometryChannel( IPluginConstPtr prev );

public:

    explicit                                    SimpleTexturePlugin         ( IPluginConstPtr prev, const std::vector< const TextureDescriptor > & textureDescs, TextureAttachmentMode amode = TextureAttachmentMode::MM_ATTACHED );
    explicit                                    SimpleTexturePlugin         ( IPluginConstPtr prev, const std::vector< const TextureDescriptor > & textureDescs, model::RendererContext * ctx = nullptr, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                                ~SimpleTexturePlugin        ();

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;                                                                           
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;                                       
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;     

    virtual TextureInfoVec                      GetTextures                 () const;

    void                                        SetAttachmentMode           ( TextureAttachmentMode mode );
    void                                        SetWrappingMode             ( TextureWrappingMode mode );

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv
