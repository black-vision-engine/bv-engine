#pragma once

#include "IDeserializer.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"

namespace bv {

class BVDeserializeContext : public DeserializeContext
{
public:
    ~BVDeserializeContext() {}

    model::OffsetTimeEvaluatorPtr               m_sceneTimeline;
};

}
