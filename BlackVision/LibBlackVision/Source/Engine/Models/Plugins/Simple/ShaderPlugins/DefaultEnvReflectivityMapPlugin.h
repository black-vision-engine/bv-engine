#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultEnvReflectivityMapPluginDesc : public BasePluginDescriptor
{
public:
    DefaultEnvReflectivityMapPluginDesc                         ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();
    static  std::string                     TextureName         ();
};


// ***************************** PLUGIN ********************************** 



class DefaultEnvReflectivityMapPlugin : public BasePlugin
{
private:

    DefaultPixelShaderChannelPtr            m_pixelShaderChannel;
    DefaultPluginParamValModelPtr           m_paramValModel;

public:
    explicit                                DefaultEnvReflectivityMapPlugin ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                            ~DefaultEnvReflectivityMapPlugin();


    virtual IPixelShaderChannelPtr          GetPixelShaderChannel       () const override;
    virtual void                            Update                      ( TimeType t ) override;
    virtual bool							SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual bool                            LoadResource                ( AssetDescConstPtr assetDescr ) override;
};


} // model
} // bv
