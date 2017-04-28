#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultAnimationPluginDesc : public BasePluginDescriptor
{
public:

    DefaultAnimationPluginDesc                                    ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultAnimationPlugin : public BasePlugin
{
public:

    static const std::string        PARAM_BLEND_ENABLE;
    static const std::string        PARAM_AUTO_PLAY;
    static const std::string        PARAM_FPS;
    
    static const std::string        PARAM_ALPHA;
    static const std::string        PARAM_FRAME_NUM;

private:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    ParamFloatPtr                   m_paramFrameNum;
    ParamBoolPtr                    m_paramAutoPlay;
    ParamFloatPtr                   m_paramFPS;

    UInt32                          m_texturesNum;

public:

    explicit                                    DefaultAnimationPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultAnimationPlugin     ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

private:

    void                                        InitVertexAttributesChannel ();

    void                                        FrameUpdate                 ();

};

} // model
} // bv
