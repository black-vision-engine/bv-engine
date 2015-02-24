#pragma once

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultGeometryPluginDescBase : public BasePluginDescriptor
{
public:

    DefaultGeometryPluginDescBase                                    ( const std::string & uid, const std::string & name );

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    //virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();
};

class DefaultGeometryPluginBase :
    public BasePlugin< IPlugin >
{
    VertexAttributesChannelPtr          m_vaChannel;

    //int											m_lastN;
    float										m_angleStart;
    float										m_angleEnd;

    void										InitGeometry( float angleStart, float angleEnd );
public:
    DefaultGeometryPluginBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
    ~DefaultGeometryPluginBase(void);

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;

    virtual void								Update                      ( TimeType t );
};

} }