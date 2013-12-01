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

    std::string         textureFileName;
    ParamTransform      transformParameter;
    TextureWrappingMode wrappingModeX;
    TextureWrappingMode wrappingModeY;

    explicit TextureDescriptor  ( const std::string& txFileName
                                , const ParamTransform& txTransform = ParametersFactory::CreateParameter( "texTransform", TransformF() )
                                , TextureWrappingMode txWrappingModeX = TextureWrappingMode::TWM_REPEAT 
                                , TextureWrappingMode txWrappingModeY = TextureWrappingMode::TWM_REPEAT )
                                : textureFileName( txFileName )
                                , transformParameter( txTransform )
                                , wrappingModeX( txWrappingModeX )
                                , wrappingModeY( txWrappingModeY )
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

    void                                        SetAttachmentMode           ( TextureAttachmentMode mode );
    void                                        SetWrappingMode             ( TextureWrappingMode mode );

    virtual void                                Update                      ( TimeType t ) override;
    virtual void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
