#pragma once

#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/Interfaces/INodeLogic.h"


namespace bv {


class NodeLogicFactory : public INodeLogicFactory
{
public:

    virtual ~NodeLogicFactory() {}

    virtual model::INodeLogicPtr        CreateLogic         ( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent )                                                                  override;
    virtual model::IGizmoLogicPtr       CreateGizmoLogic    ( const std::string & gizmoName, model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, BVProjectEditor * editor )      override;

};

} //bv
