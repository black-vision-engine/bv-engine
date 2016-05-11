#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultNormalMapPluginDesc : public BasePluginDescriptor
{
public:

    DefaultNormalMapPluginDesc                                  ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultNormalMapPlugin : public BasePlugin< IPlugin >
{
protected:

    DefaultPixelShaderChannelPtr    m_psc;

public:

    explicit                                    DefaultNormalMapPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultNormalMapPlugin     ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    
    virtual void                                Update                      ( TimeType t ) override;

private:

    virtual void								SetPrevPlugin               ( IPluginPtr plugin ) override;

};

} // model
} // bv
