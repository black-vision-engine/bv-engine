#pragma once

#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Channels/DefaultFinalizePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultFinalizeVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultFinalizeGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Audio/DefaultAudioChannel.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

namespace bv { namespace model {

class DefaultFinalizePlugin : public IFinalizePlugin
{
private:

    IPluginPtr                      m_prevPlugin;

    std::string                     m_shadersDir;
   
    std::string                     m_name;
    static std::string              m_uid;

    DefaultVertexShaderChannelPtr   m_defaultVSChannel;
    ITransformChannelConstPtr		m_defaultTransformChannel;

    mutable DefaultFinalizePixelShaderChannelPtr    m_finalizePSC;
    mutable DefaultFinalizeVertexShaderChannelPtr   m_finalizeVSC;
    mutable DefaultFinalizeGeometryShaderChannelPtr m_finalizeGSC;
    mutable DefaultAudioChannelPtr                  m_finalizeAudioChannel;

public:

                                                DefaultFinalizePlugin       ();
                                                ~DefaultFinalizePlugin      ();

    virtual IPluginPtr							Clone						() const override;

    virtual UInt32				                GetID		                () const override;
    virtual const std::string &                 GetName                     () const override;
    virtual const std::string &                 GetTypeUid                  () const override;
                                                                                     
    virtual IPluginParamValModelPtr             GetPluginParamValModel      () const override;
    virtual IParameterPtr                       GetParameter                ( const std::string & name ) const override;
    virtual bv::IValueConstPtr                  GetValue                    ( const std::string & name ) const override;
    virtual ICachedParameterPtr                 GetCachedParameter          ( const std::string & name ) const override;
    
    virtual IParamValModelPtr					GetResourceStateModel		( const std::string & name ) const override;
    virtual std::vector< IParameterPtr >        GetResourceStateModelParameters () const;

    virtual std::vector< IParameterPtr >        GetParameters               () const;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual ITransformChannelConstPtr           GetTransformChannel         () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    virtual IGeometryShaderChannelConstPtr      GetGeometryShaderChannel    () const override;
    virtual IAudioChannelPtr                    GetAudioChannel             () const override;
                 
    virtual RendererContextConstPtr             GetRendererContext          () const override;
    virtual void                                SetRendererContext          ( RendererContextPtr context ) override;

    virtual IPluginConstPtr                     GetPrevPlugin               () const override;
    virtual IPluginPtr							GetPrevPlugin               () override;
    virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & currentTransformation ) const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;
    virtual std::vector< LAsset >               GetLAssets                  () const;

    virtual void                                Update                      ( TimeType t ) override;

    virtual bool                                SetPrevPlugin               ( IPluginPtr plugin ) override;
    void                                        SetName                     ( const std::string & name );

    static std::string                          ShadersDir                  ();

    virtual ParamTransformPtr				    GetParamTransform			() const override;
    
    virtual std::vector< ITimeEvaluatorPtr >	GetTimelines				() const override;

    virtual bool								IsValid						() const override;

private:
    void										UpdateShaderChannelModel	( DefaultParamValModelPtr psModel, DefaultTexturesDataPtr txData, IPluginPtr plugin ) const;
    void										UpdateTexturesData			( DefaultPixelShaderChannelPtr psc ) const;
    
    void										AddModel					( DefaultParamValModelPtr psModel, IParamValModelPtr model ) const;
    void										AddTextures					( DefaultTexturesDataPtr txData, IPluginPtr plugin ) const;
    
    void										UpdatePixelShaderChannel	() const;

    std::vector< std::string >                  GetUIDS                    () const;

};

DEFINE_PTR_TYPE(DefaultFinalizePlugin)

} //model
}  //bv
