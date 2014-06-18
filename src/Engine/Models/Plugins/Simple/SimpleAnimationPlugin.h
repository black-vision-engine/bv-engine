#pragma once

#include "Engine/Models/Plugins/Simple/SimpleTexturePlugin.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"


namespace bv { namespace model {

class Resource;
class TexturePixelShaderChannel;
class TextureVertexShaderChannel;

// ***************************** UID **********************************
class SimpleAnimationPluginUID
{
public:

    static const char *      GetName()        { return "simple_animation_plugin"; }
};

//Just to avoid mixing multiple inheritance of interface and implementation (it is not a problem here, but this way it is cleaner)
class ISequenceAnimationSourcePlugin : public IPlugin, public ISequenceAnimationSource
{
public:

    virtual ~ISequenceAnimationSourcePlugin() {}

};

// ***************************** PLUGIN **********************************
class SimpleAnimationPlugin : public BasePlugin< ISequenceAnimationSourcePlugin >
{
private:

    VertexAttributesChannelPtr      m_vaChannel;
    TexturePixelShaderChannelPtr    m_pixelShaderChannel;
    TextureVertexShaderChannelPtr   m_vertexShaderChannel;
    TextureInfoVec                  m_textures;

    TextureAttachmentMode           m_attachmentMode;
    unsigned int                    m_texCoordChannelIndex;

    unsigned int                    m_numFrames;
    ParamFloat                      m_frameCounter;

    unsigned int                    m_curFrame;
    unsigned int                    m_nextFrame;

private:

    TextureInfo *       LoadTexture         ( const std::string & name, const std::string & path )   const;
    void                EvalGeometryChannel ( IPluginConstPtr prev );

public:

    explicit                            SimpleAnimationPlugin       ( IPluginConstPtr prev, const std::vector< std::string > & texturesFilesNames, const ParamFloat & frameCounter, model::RendererContext * ctx = nullptr, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                        ~SimpleAnimationPlugin      ();

    void                                SetAttachmentMode           ( TextureAttachmentMode mode );


    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;                                                                           
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;                                       
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    virtual TextureInfoVec                      GetTextures                 () const;

    // ISequenceAnimationSource
    virtual unsigned int                CurrentFrame                () const;
    virtual unsigned int                PredictedNextFrame          () const;

    virtual bool                        HasAnimatingTexture         () const;
    virtual ISequenceAnimationSource *  QuerySequenceAnimationSource();

    virtual void                        Update                      ( TimeType t ) override;
    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv
