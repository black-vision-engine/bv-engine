#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultEnvironmentTexturePluginDesc : public BasePluginDescriptor
{
public:
    DefaultEnvironmentTexturePluginDesc                         ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();
    static  std::string                     TextureName         ();
};


// ***************************** PLUGIN ********************************** 
/// @ingroup PluginsList
class DefaultEnvironmentTexturePlugin : public BasePlugin
{
public:

    enum MixMode
    {
        Blend = 0,
        Decal,
        Modulate,
        Add,
        Average,
        AddSigned
    };
    
    struct PARAM
    {
        static const std::string        Reflectivity;
		static const std::string        TxMat;
        static const std::string        EnvMixMode;
    };

private:

    DefaultPixelShaderChannelPtr            m_pixelShaderChannel;
    DefaultPluginParamValModelPtr           m_paramValModel;

public:
    explicit                                DefaultEnvironmentTexturePlugin ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                            ~DefaultEnvironmentTexturePlugin();


    virtual IPixelShaderChannelPtr          GetPixelShaderChannel       () const override;
    virtual void                            Update                      ( TimeType t ) override;
    virtual bool							SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual bool                            LoadResource                ( AssetDescConstPtr assetDescr ) override;
};


} // model
} // bv
