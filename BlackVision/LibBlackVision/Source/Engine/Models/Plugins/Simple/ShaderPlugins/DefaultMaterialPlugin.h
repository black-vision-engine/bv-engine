#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultMaterialPluginDesc : public BasePluginDescriptor
{
public:
    DefaultMaterialPluginDesc                                      ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

};


// ***************************** PLUGIN ********************************** 
class DefaultMaterialPlugin : public BasePlugin
{
public:
    
    struct PARAM
    {
        static const std::string    DIFFUSE;
        static const std::string    AMBIENT;
        static const std::string    SPECULAR;
        static const std::string    EMISSION;
        static const std::string    SHININESS;

    };

private:

    DefaultPixelShaderChannelPtr            m_pixelShaderChannel;
    DefaultPluginParamValModelPtr           m_paramValModel;

public:

    explicit                                DefaultMaterialPlugin       ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                            ~DefaultMaterialPlugin      ();

    virtual IPixelShaderChannelPtr          GetPixelShaderChannel       () const override;

    virtual void                            Update                      ( TimeType t ) override;

    virtual void							SetPrevPlugin               ( IPluginPtr plugin ) override;

};

} // model
} // bv
