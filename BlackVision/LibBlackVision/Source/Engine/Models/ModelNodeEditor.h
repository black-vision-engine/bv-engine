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

    BasicNodePtr			CopyNode			();

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

	void                    ReplaceTimeline		( const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline );

	void					RefreshNode			( SceneNode * sceneNode, Renderer * renderer );

private:

	void					ReplaceTimeline		( model::BasicNodePtr node, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline );
	void					ReplaceTimeline		( model::IPluginPtr plugin, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline );
	void					ReplaceTimeline		( ITexturesDataPtr txData, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline );
	void					ReplaceTimeline		( model::IParamValModelPtr paramValModel, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline );

};

} //model
} //bv
