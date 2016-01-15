#pragma once

#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/Interfaces/INodeLogic.h"


namespace bv
{


class NodeLogicFactory : public INodeLogicFactory
{
private:
public:
    virtual model::INodeLogicPtr        CreateLogic  ( model::BasicNodePtr logicParent, const IDeserializer & deser ) override;
};

} //bv
