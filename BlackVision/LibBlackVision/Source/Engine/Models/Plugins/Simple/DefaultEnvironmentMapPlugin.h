#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultEnvironmentMapPluginDesc : public BasePluginDescriptor
{
public:
    DefaultEnvironmentMapPluginDesc                             ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();
    static  std::string                     TextureName         ();
};


// ***************************** PLUGIN ********************************** 


class DefaultEnvironmentMapPlugin : public BasePlugin< IPlugin >
{
public:
    
    struct PARAM
    {
        static const std::string        Reflectivity;
    };

private:

    DefaultPixelShaderChannelPtr            m_pixelShaderChannel;
    DefaultPluginParamValModelPtr           m_paramValModel;

public:
    explicit                                DefaultEnvironmentMapPlugin ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                            ~DefaultEnvironmentMapPlugin();


    virtual IPixelShaderChannelPtr          GetPixelShaderChannel       () const override;
    virtual void                            Update                      ( TimeType t ) override;
    virtual void							SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual bool                            LoadResource                ( AssetDescConstPtr assetDescr ) override;
};


} // model
} // bv
