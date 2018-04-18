#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultParallaxMapPluginDesc : public BasePluginDescriptor
{
public:

    DefaultParallaxMapPluginDesc                                ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
/// @ingroup PluginsList
class DefaultParallaxMapPlugin : public BasePlugin
{
public:

    struct PARAM 
    {
        static const std::string    HEIGHT_SCALE;
        static const std::string    MIN_SAMPLES_NUM;
        static const std::string    MAX_SAMPLES_NUM;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;

public:

    explicit                                    DefaultParallaxMapPlugin    ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultParallaxMapPlugin   ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    
    virtual void                                Update                      ( TimeType t ) override;

private:

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

};

} // model
} // bv
