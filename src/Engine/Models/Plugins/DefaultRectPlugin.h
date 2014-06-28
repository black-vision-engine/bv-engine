#pragma once

#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

class DefaultRectPlugin;
class RectComponent;

// ***************************** DESC **********************************
class DefaultRectPluginDesc : public BasePluginDescriptor
{
public:

    DefaultRectPluginDesc                                       ();

    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginConstPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

};


// ***************************** PLUGIN ********************************** 
class DefaultRectPlugin : public BasePlugin< IPlugin >
{
private:

    //FIXME: move to base class
    DefaultPluginParamValModelPtr       m_paramValModel;
    VertexAttributesChannelPtr          m_vaChannel;

    ParamFloatPtr                       m_widthParam;
    ParamFloatPtr                       m_heightParam;

    float                               m_lastW;
    float                               m_lastH;

    RectComponentPtr                    m_rct;

public:

    explicit            DefaultRectPlugin    ( const std::string & name, const std::string & uid, IPluginConstPtr prev, DefaultPluginParamValModelPtr model, bool timeInvariantDimensions = false );
                        ~DefaultRectPlugin   ();

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual void                                Update                      ( TimeType t ) override;

};

DEFINE_PTR_TYPE(DefaultRectPlugin)
DEFINE_CONST_PTR_TYPE(DefaultRectPlugin)

} // model
} // bv
