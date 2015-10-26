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
class DefaultAnimationPluginDesc : public BasePluginDescriptor
{
public:

    DefaultAnimationPluginDesc                                    ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultAnimationPlugin : public BasePlugin< IPlugin >
{
private:

    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    ParamFloatPtr                   m_paramFrameNum;
    ParamFloatPtr                   m_paramWrapModeX;
    ParamFloatPtr                   m_paramWrapModeY;
    ParamFloatPtr                   m_paramFilteringMode;
    ParamFloatPtr                   m_paramAttachMode;

    TextureWrappingMode             m_lastTextureWrapModeX;
    TextureWrappingMode             m_lastTextureWrapModeY;
    TextureFilteringMode            m_lastTextureFilteringMode;
    TextureAttachmentMode           m_lastTextureAttachMode;

public:

    explicit                                    DefaultAnimationPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultAnimationPlugin     ();

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;
private:

    void                                        InitAttributesChannel       ( IPluginPtr prev );

    TextureWrappingMode                         GetWrapModeX                () const;
    TextureWrappingMode                         GetWrapModeY                () const;
    TextureFilteringMode                        GetFilteringMode            () const;
    TextureAttachmentMode                       GetAttachementMode          () const;

    bool                                        StateChanged                ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am ) const;
    void                                        UpdateState                 ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am );

};

} // model
} // bv
