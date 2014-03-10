#pragma once

#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model {

class DefaultRectPlugin;
class RectComponent;

// ***************************** DESC **********************************
class DefaultRectPluginDesc : public BasePluginDescriptor
{
public:

    DefaultRectPluginDesc                                       ();

    virtual bool                            CanBeAttachedTo     ( const IPlugin * plugin )  const override;
    virtual IPlugin *                       CreatePlugin        ( const std::string & name, const IPlugin * prev ) const override;
    virtual DefaultPluginParamValModel *    CreateDefaultModel  () const override;
   
    static  std::string                     UID                 ();

};


// ***************************** PLUGIN ********************************** 
class DefaultRectPlugin : public BasePlugin< IPlugin >
{
private:

    //FIXME: move to base class
    DefaultPluginParamValModelPtr       m_paramValModel;
    VertexAttributesChannel *           m_vaChannel;

    ParamFloat *                        m_widthParam;
    ParamFloat *                        m_heightParam;

    float                               m_lastW;
    float                               m_lastH;

    RectComponent *                     m_rct;

public:

    explicit            DefaultRectPlugin    ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model );
                        ~DefaultRectPlugin   ();

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;
    virtual void                                Update                      ( TimeType t ) override;

};

} // model
} // bv
