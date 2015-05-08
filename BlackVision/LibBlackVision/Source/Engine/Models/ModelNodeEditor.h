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

    bool                    AddPlugin			( IPluginPtr plugin, unsigned int idx );
    bool                    DeletePlugin		( unsigned int idx );
    bool                    DeletePlugin		( const std::string & name );

    bool                    AttachPlugin		( unsigned int idx );
    bool                    DetachPlugin		( unsigned int idx );
    bool                    DetachPlugin		( const std::string & name );

	IPluginPtr				GetDetachedPlugin	();
    void                    ResetDetachedPlugin	();

	void					RefreshNode			( SceneNode * sceneNode, Renderer * renderer );

};

} //model
} //bv
