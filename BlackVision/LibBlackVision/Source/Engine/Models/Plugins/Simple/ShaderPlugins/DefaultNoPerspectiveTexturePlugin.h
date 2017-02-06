#pragma once

#include "DefaultTexturePlugin.h"


namespace bv { namespace model {



// ***************************** DESCRIPTOR **********************************
class DefaultNoPerspectiveTexturePluginDesc : public DefaultTexturePluginDesc
{
public:

	DefaultNoPerspectiveTexturePluginDesc                      ();

	virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
	virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
	static  std::string                     UID                 ();

	static  std::string                     TextureName         ();

};


/**
Plugin uses shaders, that don't do perspective interpolation of UVs coordinates.*/
class DefaultNoPerspectiveTexturePlugin :	public DefaultTexturePlugin
{
public:
	DefaultNoPerspectiveTexturePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
	virtual ~DefaultNoPerspectiveTexturePlugin();
};


}} // model // bv
