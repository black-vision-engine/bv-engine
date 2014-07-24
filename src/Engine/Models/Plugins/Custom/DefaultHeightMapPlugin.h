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

    static  std::string                     HeightMapTextureName    ();
    static  std::string                     HillTextureName         ();
    static  std::string                     CoveredDistTextureName  ();
    static  std::string                     BackgroundTextureName   ();

};

// ***************************** PLUGIN ********************************** 
class DefaultHeightMapPlugin : public BasePlugin< IPlugin >
{
private:

    enum class TextureSlot : int
    {
        TS_HEIGHT_MAP = 0,
        TS_HILL,
        TS_COVERED_DIST,
        TS_BACKGROUND,

        TS_TOTAL
    };

private:

    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    unsigned int                    m_texCoordChannelIndex;

public:

    explicit                                    DefaultHeightMapPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultHeightMapPlugin     ();

    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:

    void                                        InitAttributesChannel       ( IPluginPtr prev );
    void                                        SetTextureParams            ( TextureSlot slot, DefaultTextureDescriptor * txDesc ) const;

};

} // model
} // bv
