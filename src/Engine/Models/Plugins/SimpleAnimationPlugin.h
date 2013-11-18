#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

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

    GeometryChannel*            m_geomChannel;
    TexturePixelShaderChannel*  m_pixelShaderChannel;
    TextureVertexShaderChannel* m_vertexShaderChannel;

    TextureAttachmentMode       m_attachmentMode;
    unsigned int                m_texCoordChannelIndex;

    mutable float           m_startTime;
    float                   m_secsPerFrame;

    unsigned int            m_numFrames;
    unsigned int            m_fps;

private:

    TextureInfo *       LoadTexture         ( const std::string & name, const std::string & path )   const;
    void                EvalGeometryChannel ( const IPlugin* prev );

public:

    explicit                            SimpleAnimationPlugin       ( const IPlugin * prev, const std::vector< std::string > & texturesFilesNames, unsigned int fps, TextureAttachmentMode mode = TextureAttachmentMode::MM_ATTACHED );
                                        ~SimpleAnimationPlugin      ();

    void                                SetAttachmentMode           ( TextureAttachmentMode mode );


    virtual const IGeometryChannel *            GetGeometryChannel          () const override;                                                                           
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;                                       
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;                                       

    // ISequenceAnimationSource
    virtual unsigned int                CurrentFrame                ( float t ) const;
    virtual unsigned int                PredictedNextFrame          ( float t ) const;

    virtual bool                        HasAnimatingTexture         () const;
    virtual ISequenceAnimationSource *  QuerySequenceAnimationSource();

    void                                Update                      ( TimeType t ) override;
    void                                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv
