#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"

#include "Engine/Models/Plugins/PluginEnums.h"
#include "Engine/Models/Plugins/SimpleTexturePlugin.h"

#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"


namespace bv { namespace model {

class Resource;
class TexturePixelShaderChannel;
class TextureVertexShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleAnimationPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleAnimationPluginPD();
};

//Just to avoid mixing multiple inheritance of interface and implementation (it is not a problem here, but this way it is cleaner)
class ISequenceAnimationSourcePlugin : public IPlugin, public ISequenceAnimationSource
{
public:

    virtual ~ISequenceAnimationSourcePlugin() {}

};

// ***************************** PLUGIN **********************************
class SimpleAnimationPlugin : public BasePlugin< ISequenceAnimationSourcePlugin, SimpleAnimationPluginPD >
{
private:

    VertexAttributesChannel *       m_geomChannel;
    TexturePixelShaderChannel *     m_pixelShaderChannel;
    TextureVertexShaderChannel *    m_vertexShaderChannel;

    TextureAttachmentMode           m_attachmentMode;
    unsigned int                    m_texCoordChannelIndex;

    unsigned int                    m_numFrames;
    ParamFloat                      m_frameCounter;

private:

    TextureInfo *       LoadTexture         ( const std::string & name, const std::string & path )   const;
    void                EvalGeometryChannel ( const IPlugin* prev );

public:

    explicit                            SimpleAnimationPlugin       ( const IPlugin * prev, const std::vector< std::string > & texturesFilesNames, const ParamFloat & frameCounter, model::RendererContext * ctx = nullptr, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                        ~SimpleAnimationPlugin      ();

    void                                SetAttachmentMode           ( TextureAttachmentMode mode );


    virtual const IVertexAttributesChannel *    GetGeometryChannel          () const override;                                                                           
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;                                       
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;                                       

    // ISequenceAnimationSource
    virtual unsigned int                CurrentFrame                ( TimeType t ) const;
    virtual unsigned int                PredictedNextFrame          ( TimeType t ) const;

    virtual bool                        HasAnimatingTexture         () const;
    virtual ISequenceAnimationSource *  QuerySequenceAnimationSource();

    virtual void                        Update                      ( TimeType t ) override;
    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
