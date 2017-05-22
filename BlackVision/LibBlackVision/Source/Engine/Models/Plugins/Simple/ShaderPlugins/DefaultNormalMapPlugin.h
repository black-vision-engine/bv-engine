#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

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
class DefaultNormalMapPlugin : public BasePlugin
{
public:

    struct PARAM 
    {
        static const std::string    NORMAL_MAP_MAT;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

public:

    explicit                                    DefaultNormalMapPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultNormalMapPlugin     ();

    virtual bool                                IsValid						() const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    
    virtual void                                Update                      ( TimeType t ) override;

private:

    virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;

};

} // model
} // bv
