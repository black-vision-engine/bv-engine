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


// ***************************** PLUGIN ********************************** 
class SimpleTransformPlugin : public BasePlugin< IPlugin, SimpleTransformPluginUID >
{
private:

    SimpleTransformChannelPtr               m_transformChannel;

public:

    explicit                                SimpleTransformPlugin       ( const IPlugin * prev, const ParamTransformVec & transformVec );
                                            ~SimpleTransformPlugin      ();

    virtual const ITransformChannel *       GetTransformChannel         () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const override;

    //void                                    OnSetTransform              ( IEventPtr evt );

    static  SimpleTransformPlugin *         Create                      ( const IPlugin * prev, const ParamTransformVec & transformVec );

};

} // model
} // bv
