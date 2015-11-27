#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Models/Plugins/DefaultFinalizePlugin.h"


namespace bv { namespace model {

class ModelNodeEditor
{
private:
	BasicNodeWeakPtr	m_node;
    IPluginPtr			m_detachedPlugin;

public:
                            ModelNodeEditor		( BasicNodePtr node ); 

    bool                    AddPlugin			( IPluginPtr plugin, unsigned int idx );
    bool                    DeletePlugin		( unsigned int idx );
    bool                    DeletePlugin		( const std::string & name );

    bool                    AttachPlugin		( unsigned int idx );
    bool                    AttachPlugin		( BasicNodePtr sourceNode, unsigned int idx );
    bool                    DetachPlugin		( unsigned int idx );
    bool                    DetachPlugin		( const std::string & name );

    IPluginPtr              CopyPlugin			( const std::string & name );

	IPluginPtr				GetDetachedPlugin	();
    void                    ResetDetachedPlugin	();

	IModelNodeEffectPtr		GetNodeEffect		();
    void					SetNodeEffect		( IModelNodeEffectPtr nodeEffect );


	void					RefreshNode			( SceneNode * sceneNode, Renderer * renderer );

};

} //model
} //bv
