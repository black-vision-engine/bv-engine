#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class ExpertPluginDesc : public BasePluginDescriptor
{
public:

    ExpertPluginDesc                               				();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class ExpertPlugin : public BasePlugin
{
public:

    struct PARAMS
    {
        static const std::string        ALPHA;
        static const std::string        BLENDING_MODE;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

public:

    explicit                                    ExpertPlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    virtual                                     ~ExpertPlugin  ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:
    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;
};


} // model
} // bv


