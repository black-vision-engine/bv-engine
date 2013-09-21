#pragma once

#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"

#include <vector>

namespace bv { namespace model
{

class Transform;

class TransformChannel : public ITransformChannel
{
    std::vector< Transform* >         m_transformations;

public:

};

} // model
} // bv
