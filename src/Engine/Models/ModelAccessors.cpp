#include "ModelAccessors.h"

#include "Engine/Models/ModelScene.h"
#include "Engine/Models/BasicNode.h"

namespace bv { namespace model {

// ********************************
//
BasicNode *     ModelAccessors::GetRootNode    ( ModelScene * scene )
{
    return scene->m_pSceneRoot;
}

// ********************************
//
BasicNode *     ModelAccessors::FindChild   ( BasicNode * node, const std::string & name )
{
    for( auto n : node->m_children )
    {
        if( n->GetName() == name )
        {
            return static_cast< BasicNode * >( n.get() );
        }
    }

    return nullptr;
}

// ********************************
//
BasicNode *     ModelAccessors::FindLayer   ( BasicNode * node, const std::string & name )
{
    for( auto n : node->m_layers )
    {
        if( n->GetName() == name )
        {
            return static_cast< BasicNode * >( n.get() );
        }
    }

    return nullptr;
}

} //model
} //bv

