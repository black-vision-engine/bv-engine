#include "stdafx.h"

#include "DefaultNoPerspectiveTexturePlugin.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

	// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultNoPerspectiveTexturePluginDesc::DefaultNoPerspectiveTexturePluginDesc                          ()
    : DefaultTexturePluginDesc( UID(), "texture", "nptx" )
{
}

// *******************************
//
IPluginPtr              DefaultNoPerspectiveTexturePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTexturePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultNoPerspectiveTexturePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    return DefaultTexturePluginDesc::CreateDefaultModel( timeEvaluator );
}

// *******************************
//
std::string             DefaultNoPerspectiveTexturePluginDesc::UID                       ()
{
    return "DEFAULT_NO_PERSPECTIVE_TEXTURE";
}

// *******************************
// 
std::string             DefaultNoPerspectiveTexturePluginDesc::TextureName               ()
{
    return "Tex0";
}



// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultNoPerspectiveTexturePlugin::DefaultNoPerspectiveTexturePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
	: DefaultTexturePlugin( name, uid, prev, model )
{}

// *************************************
// 
DefaultNoPerspectiveTexturePlugin::~DefaultNoPerspectiveTexturePlugin()
{}


}} // model // bv
