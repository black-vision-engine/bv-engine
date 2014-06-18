#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Events/Events.h"


namespace bv { namespace model {

class SimpleTransformChannel;
class ParamTransform;

// ***************************** UID **********************************
class SimpleTransformPluginUID
{
public:

    static const char * GetName ()    { return "simple_transform_plugin"; }

};


class SimpleTransformPlugin;
DEFINE_PTR_TYPE(SimpleTransformPlugin)

// ***************************** PLUGIN ********************************** 
class SimpleTransformPlugin : public BasePlugin< IPlugin >
{
private:

    SimpleTransformChannelPtr               m_transformChannel;

public:

    explicit                                SimpleTransformPlugin       ( IPluginConstPtr prev, const ParamTransformVec & transformVec );
                                            ~SimpleTransformPlugin      ();

    virtual ITransformChannelConstPtr       GetTransformChannel         () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const;

    //void                                    OnSetTransform              ( IEventPtr evt );

    static  SimpleTransformPluginPtr        Create                      ( IPluginConstPtr prev, const ParamTransformVec & transformVec );

};

} // model
} // bv
