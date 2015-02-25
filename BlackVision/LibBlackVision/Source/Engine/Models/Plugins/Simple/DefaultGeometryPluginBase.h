#pragma once

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultGeometryPluginDescBase : public BasePluginDescriptor
{
public:

    DefaultGeometryPluginDescBase                                    ( const std::string & uid, const std::string & name );

    //virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();
};

class DefaultGeometryPluginBase :
    public BasePlugin< IPlugin >
{
    VertexAttributesChannelPtr          m_vaChannel;
    //IGeometryAndUVsGenerator&           m_generator;

    virtual void								InitGeometry();
    virtual IGeometryAndUVsGenerator*           GetGenerator() { assert( false ); return nullptr; } // FIXME
public:
    DefaultGeometryPluginBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
    //~DefaultGeometryPluginBase(void);

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;

    virtual void								Update                      ( TimeType t );
};

} }