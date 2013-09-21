#pragma once

#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include <vector>

namespace bv { namespace model
{

class GeometryChannel : public IGeometryChannel
{
    std::vector< IConnectedComponent* >         m_components;

public:

};

} // model
} // bv
