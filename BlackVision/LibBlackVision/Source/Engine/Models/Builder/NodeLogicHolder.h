#pragma once

#include "Serialization/IDeserializer.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/Interfaces/IGizmoLogic.h"
#include "Engine/Models/BasicNode.h"

namespace bv
{

// Implement INodeLogic creation in derived class.
class INodeLogicFactory
{
private:
public:
    virtual ~INodeLogicFactory() {};


    virtual model::INodeLogicPtr        CreateLogic         ( const IDeserializer & /*deser*/, model::BasicNodeWeakPtr /*logicParent*/ )
    { return nullptr; }

    virtual model::IGizmoLogicPtr       CreateGizmoLogic    ( const std::string & gizmoName, model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner )
    {   gizmoName; gizmoRoot; gizmoOwner; return nullptr;    }
};


// Sets node logic factory.
// Factory will be deleted automatically
void                    SetNodeLogicFactory         ( INodeLogicFactory * factory );
INodeLogicFactory *     GetNodeLogicFactory         ();

} //bv
