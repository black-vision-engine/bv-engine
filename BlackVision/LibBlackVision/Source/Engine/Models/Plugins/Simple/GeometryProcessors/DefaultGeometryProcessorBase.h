#pragma once

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultGeometryProcessorDescBase : public BasePluginDescriptor
{
public:

                                            DefaultGeometryProcessorDescBase   ( const std::string & uid, const std::string & name );

    static  std::string                     UID                             ();

};

// ***************************** PLUGIN ********************************** 
class DefaultGeometryProcessorBase : public BasePlugin
{
protected:

    VertexAttributesChannelPtr                  m_vaChannel;

protected:

    virtual void                                ProcessVertexAttributesChannel  ();

    // Processes one connected component. This function will be executed as many times as many connected components there are.
    // Function can use parameter allComponents as context for processing.
    virtual void                                ProcessConnectedComponent       (   model::ConnectedComponentPtr & currComponent,
                                                                                    std::vector< IConnectedComponentPtr > & allComponents,
                                                                                    PrimitiveType topology ) = 0;

    virtual void                                InitializeVertexAttributesChannel ();

public:

    DefaultGeometryProcessorBase                ( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual bool                                SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual void                                Update                      ( TimeType t );
};

} // model
} // bv