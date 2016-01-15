#pragma once

#include "Serialization/IDeserializer.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/BasicNode.h"

namespace bv
{

// Implement INodeLogic creation in derived class.
class INodeLogicFactory
{
private:
public:
    virtual ~INodeLogicFactory();


    virtual model::INodeLogicPtr        CreateLogic  ( model::BasicNodePtr /*logicParent*/, const IDeserializer & /*deser*/ ) { return nullptr; }
};


// Sets node logic factory.
// Factory will be deleted automatically
void    SetNodeLogicFactory         ( INodeLogicFactory * factory );

} //bv
