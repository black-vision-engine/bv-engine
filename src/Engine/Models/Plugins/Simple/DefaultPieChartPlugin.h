#pragma once

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultPieChartPluginDesc : public BasePluginDescriptor
{
public:

    DefaultPieChartPluginDesc                                    ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();
};

class DefaultPieChartPlugin :
	public BasePlugin< IPlugin >
{
    VertexAttributesChannelPtr          m_vaChannel;

	int											m_lastN;
	void										InitGeometry( int n );
public:
	DefaultPieChartPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
	~DefaultPieChartPlugin(void);

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;

	virtual void								Update                      ( TimeType t );
};

} }