#pragma once

#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/Interfaces/INodeLogic.h"


namespace bv
{


class NodeLogicFactory : public INodeLogicFactory
{
private:
public:
    virtual model::INodeLogicPtr        CreateLogic  (  const std::string&              logicType,
                                                        model::BasicNodePtr             logicParent,
                                                        bv::model::ITimeEvaluatorPtr    timeEvaluator,
                                                        const IDeserializer &           deser           ) override;
};

} //bv
