#pragma once

#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

// ***************************** Desc **********************************
class DefaultTransformPluginDesc : public BasePluginDescriptor
{
public:

    DefaultTransformPluginDesc                                  ();

    virtual IPlugin *                       CreatePlugin        ( const std::string & name, const IPlugin * prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

};


// ***************************** PLUGIN ********************************** 
class DefaultTransformPlugin : public BasePlugin< IPlugin >
{
private:

    //FIXME: move to base class
    DefaultTransformChannelPtr                  m_transformChannel;
    DefaultPluginParamValModelPtr               m_paramValModel;

public:

    explicit                                    DefaultTransformPlugin      ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTransformPlugin     ();

    virtual const ITransformChannel *           GetTransformChannel         () const override;
    virtual void                                Update                      ( TimeType t ) override;

};

} // model
} // bv
