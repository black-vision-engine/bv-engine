#pragma once

#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/Interfaces/INodeLogic.h"


namespace bv
{


class NodeLogicFactory : public INodeLogicFactory
{
private:
public:
    virtual ~NodeLogicFactory() {}

    virtual model::INodeLogicPtr        CreateLogic  (  const IDeserializer &   deser,
                                                        model::BasicNodePtr     logicParent
                                                     )  override;
};

} //bv
