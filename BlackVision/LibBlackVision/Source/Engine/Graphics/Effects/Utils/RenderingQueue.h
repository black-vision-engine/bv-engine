#pragma once


#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv
{


class RenderingQueue
{
private:

    std::vector< SceneNode * >          m_sortedNodes;

public:
    RenderingQueue();
    ~RenderingQueue();
};


}	// bv
