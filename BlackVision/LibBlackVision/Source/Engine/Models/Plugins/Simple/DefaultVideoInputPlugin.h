#pragma once
#include "Engine\Models\Plugins\Simple\DefaultTexturePlugin.h"
//#include "DefaultVideoInput.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultVideoInputPluginDesc : public BasePluginDescriptor
{
public:

	DefaultVideoInputPluginDesc                                    ();

	virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
	virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

	virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

	static  std::string                     UID                 ();

	static  std::string                     VertexShaderSource  ();
	static  std::string                     PixelShaderSource   ();

	static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultVideoInputPlugin :
	public DefaultTexturePlugin
{
	std::vector< IVideoInputResourceDescrConstPtr > vis;

    int                                     GetSourceNumber();
public:
	DefaultVideoInputPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
	~DefaultVideoInputPlugin(void);

	virtual void                                Update                      ( TimeType t ) override;
	virtual bool                            LoadResource  ( IPluginResourceDescrConstPtr resDescr ) override;
};

} }