#pragma once

#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultVideoInputPluginDesc : public DefaultTexturePluginDesc
{
public:

	DefaultVideoInputPluginDesc                                    ();

	virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
	virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

	static  std::string                     UID                 ();

	static  std::string                     VertexShaderSource  ();
	static  std::string                     PixelShaderSource   ();

	static  std::string                     TextureName         ();

};

// ***************************** PLUGIN ********************************** 
class DefaultVideoInputPlugin :
	public DefaultTexturePlugin
{

    int                                     GetSourceNumber();

public:
	DefaultVideoInputPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
	~DefaultVideoInputPlugin(void);

	virtual void                                Update                      ( TimeType t ) override;
    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;
};

} }