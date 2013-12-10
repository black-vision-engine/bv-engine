#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"
#include "Engine/Events/Events.h"


namespace bv { namespace model {

class SimpleTransformChannel;
class ParamTransform;

// ***************************** DESCRIPTOR **********************************
class SimpleTransformPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;

    explicit SimpleTransformPluginPD();
};


// ***************************** PLUGIN ********************************** 
class SimpleTransformPlugin : public BasePlugin< IPlugin, SimpleTransformPluginPD >
{
private:

    SimpleTransformChannel *                m_transformChannel;

public:

    explicit                                SimpleTransformPlugin       ( const IPlugin * prev, const ParamTransform & trans );
                                            ~SimpleTransformPlugin      ();

    virtual const ITransformChannel *       GetTransformChannel         () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const override;

    void                                    OnSetTransform              ( IEventPtr evt );

    static  SimpleTransformPlugin *         Create                      ( const IPlugin * prev, const ParamTransform & trans );

};

} // model
} // bv
