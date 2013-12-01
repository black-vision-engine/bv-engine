#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"

#include "Engine/Models/Plugins/PluginEnums.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


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
                                ,    const ParamTransform & txTransform = ParametersFactory::CreateParameter( "texTransform", TransformF() )
                                ,    const ParamFloat& txAlpha = ParametersFactory::CreateParameter( "texAlpha", InterpolatorsHelper::CreateConstValue( 0.f ) )
                                ,    const ParamVec4& txBorderColor = ParametersFactory::CreateParameter( "texBorderColor", InterpolatorsHelper::CreateConstValue( glm::vec4( 0.f, 0.f, 1.f, 0.f ) ) )
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
class SimpleTexturePlugin : public BasePlugin< IPlugin, SimpleTexturePluginPD >
{
private:
    TextureAttachmentMode           m_attachmentMode;
    unsigned int                    m_texCoordChannelIndex;
    VertexAttributesChannel*        m_geomChannel;
    TexturePixelShaderChannel*      m_pixelShaderChannel;
    TextureVertexShaderChannel*     m_vertexShaderChannel;
    Textures                        m_textures;

private:

    TextureInfo *                               LoadTexture( const TextureDescriptor & texDesc, const std::string& name )   const;
    void                                        EvalGeometryChannel( const IPlugin* prev );

public:

    explicit                                    SimpleTexturePlugin         ( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, TextureAttachmentMode amode = TextureAttachmentMode::MM_ATTACHED );
    explicit                                    SimpleTexturePlugin         ( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, model::RendererContext * ctx = nullptr, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                                ~SimpleTexturePlugin        ();

    virtual const IVertexAttributesChannel*     GetGeometryChannel          () const override;                                                                           
    virtual const IPixelShaderChannel*          GetPixelShaderChannel       () const override;                                       
    virtual const IVertexShaderChannel*         GetVertexShaderChannel      () const override;     

    virtual Textures                            GetTextures                 () const override;

    void                                        SetAttachmentMode           ( TextureAttachmentMode mode );
    void                                        SetWrappingMode             ( TextureWrappingMode mode );

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
