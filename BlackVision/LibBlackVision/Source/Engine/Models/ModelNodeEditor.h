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
                            ModelNodeEditor		( model::BasicNodePtr model ); 

    void                    AddPlugin			( model::IPluginPtr plugin );
    void                    AddPlugin			( const std::string & uid, const std::string & name );
    bool                    DeletePlugin		( const std::string & name );

    void                    AttachPlugin		();
    bool                    DetachPlugin		( const std::string & name );

	IPluginPtr				GetDetachedPlugin     ();
    void                    DeleteDetachedPlugin  ();
};

} //model
} //bv
