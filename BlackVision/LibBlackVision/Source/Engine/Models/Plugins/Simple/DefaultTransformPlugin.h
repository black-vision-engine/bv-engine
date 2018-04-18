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

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

};


// ***************************** PLUGIN ********************************** 
/// @ingroup PluginsList
class DefaultTransformPlugin : public BasePlugin
{

public:
    
    struct PARAM
    {
        static const std::string    SIMPLE_TRANSFORM;
    };

private:

    //FIXME: move to base class
    DefaultTransformChannelPtr                  m_transformChannel;
    DefaultPluginParamValModelPtr               m_paramValModel;

public:

    explicit                                    DefaultTransformPlugin      ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTransformPlugin     ();

    virtual ITransformChannelConstPtr           GetTransformChannel         () const override;
    virtual void                                Update                      ( TimeType t ) override;

	virtual ParamTransformPtr				    GetParamTransform			() const override;

};

} // model
} // bv
