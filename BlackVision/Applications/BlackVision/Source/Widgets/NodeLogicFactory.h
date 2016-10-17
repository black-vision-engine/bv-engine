#pragma once

#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/Interfaces/INodeLogic.h"


namespace bv {


class NodeLogicFactory : public INodeLogicFactory
{
public:

    virtual ~NodeLogicFactory() {}

    virtual model::INodeLogicPtr        CreateLogic  (  const IDeserializer &   deser,
                                                        model::BasicNodeWeakPtr logicParent
                                                     )  override;

};

} //bv
