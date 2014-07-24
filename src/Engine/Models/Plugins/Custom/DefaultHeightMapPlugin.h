#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultHeightMapPluginDesc : public BasePluginDescriptor
{
public:

    DefaultHeightMapPluginDesc                                      ();

    virtual IPluginPtr                      CreatePlugin            ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel      ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo         ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                     ();

    static  std::string                     HillTextureName         ();
    static  std::string                     BackgroundTextureName   ();
    static  std::string                     BgMaskTextureName       ();
    static  std::string                     HeightMapTextureName    ();

};

// ***************************** PLUGIN ********************************** 
class DefaultHeightMapPlugin : public BasePlugin< IPlugin >
{
private:

    enum class TextureSlot : int
    {
        TS_HEIGHT_MAP = 0,
        TS_HILL,
        TS_BACKGROUND,
        TS_BACKGROUND_MASK,

        TS_TOTAL
    };

private:

    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    unsigned int                    m_texCoordChannelIndex;

    ParamFloatPtr                   m_paramWrapModeX;
    ParamFloatPtr                   m_paramWrapModeY;
    ParamFloatPtr                   m_paramFilteringMode;
    ParamFloatPtr                   m_paramAttachMode;

    unsigned int                    m_textureWidth;
    unsigned int                    m_textureHeight;

    TextureWrappingMode             m_lastTextureWrapModeX;
    TextureWrappingMode             m_lastTextureWrapModeY;
    TextureFilteringMode            m_lastTextureFilteringMode;
    TextureAttachmentMode           m_lastTextureAttachMode;

public:

    explicit                                    DefaultHeightMapPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultHeightMapPlugin     ();

    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    unsigned int                                GetTextureWidth             () const;
    unsigned int                                GetTextureHeight            () const;

    virtual void                                Update                      ( TimeType t ) override;

private:

    void                                        InitAttributesChannel       ( IPluginPtr prev );
    void                                        SetTextureParams            ( TextureSlot slot, DefaultTextureDescriptor * txDesc ) const;

};

} // model
} // bv
