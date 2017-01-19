#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Models/Plugins/DefaultFinalizePlugin.h"


namespace bv { namespace model {

class ModelNodeEditor
{
private:

    BasicNode *	        m_node;
    IPluginPtr			m_detachedPlugin;
    int                 m_detachedPluginIdx;

public:

                            ModelNodeEditor		( BasicNode * node ); 

    BasicNodePtr			CopyNode			();

    bool                    AddPlugin			( IPluginPtr plugin, UInt32 & idx );
    bool                    DeletePlugin		( unsigned int idx );
    model::PluginWithIdx    DeletePlugin		( const std::string & name );

    bool                    AttachPlugin		( UInt32 & idx );
    bool                    AttachPlugin		( BasicNodePtr sourceNode, UInt32 & idx );
    bool                    DetachPlugin		( unsigned int idx );
    bool                    DetachPlugin		( const std::string & name );

    IPluginPtr              CopyPlugin			( const std::string & name );

    IPluginPtr				GetDetachedPlugin	();
    int                     GetDetachedPluginIdx ();
    void                    ResetDetachedPlugin	();

    IModelNodeEffectPtr		GetNodeEffect		();
    void					SetNodeEffect		( IModelNodeEffectPtr nodeEffect );

    void                    ReplaceTimeline		( const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline );
    bool                    IsTimelineUsed      ( model::ITimeEvaluatorPtr timeEval );

};

} //model
} //bv
