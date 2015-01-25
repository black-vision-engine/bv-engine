#include "ModelAccessors.h"

#include "Engine/Models/ModelScene.h"
#include "Engine/Models/BasicNode.h"

namespace bv { namespace model {

// ********************************
//
BasicNodePtr     ModelAccessors::GetRootNode    ( ModelScenePtr scene )
{
    return scene->m_pSceneRoot;
}

// ********************************
//
BasicNodePtr     ModelAccessors::FindChild   ( BasicNodePtr node, const std::string & name )
{
    for( auto n : node->m_children )
    {
        if( n->GetName() == name )
        {
            return std::static_pointer_cast< BasicNode >( n );
        }
    }

    return nullptr;
}

// ********************************
//
BasicNodePtr     ModelAccessors::FindLayer   ( BasicNodePtr node, const std::string & name )
{
    for( auto n : node->m_layers )
    {
        if( n->GetName() == name )
        {
            return std::static_pointer_cast< BasicNode >( n );
        }
    }

    return nullptr;
}

} //model
} //bv

