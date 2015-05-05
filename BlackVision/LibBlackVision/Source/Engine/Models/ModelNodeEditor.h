#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Models/Plugins/DefaultFinalizePlugin.h"


namespace bv { namespace model {

class ModelNodeEditor
{
private:
	BasicNodePtr		m_model;
    IPluginPtr			m_detachedPlugin;

public:
                            ModelNodeEditor		( BasicNodePtr model ); 

    bool                    AddPlugin			( IPluginPtr plugin );
    bool                    DeletePlugin		( const std::string & name );

    bool                    AttachPlugin		();
    bool                    DetachPlugin		( const std::string & name );

	IPluginPtr				GetDetachedPlugin     ();
    void                    DeleteDetachedPlugin  ();

	void					RefreshNode ( SceneNode * sceneNode, Renderer * renderer );

private:
	bool					PluginCanBeAttached ( IPluginPtr plugin );

};

} //model
} //bv
