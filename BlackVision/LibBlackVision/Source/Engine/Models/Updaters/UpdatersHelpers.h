#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

namespace bv
{



class UpdatersHelpers
{
public:

    static void         WriteVertexDataToVBO        ( char * data, model::IConnectedComponentPtr cc );
    static void         UpdateRenderableBuffer      ( RenderableEntity * renderable, model::IConnectedComponentPtr cc );
    static void         RecreateRenderableBuffer    ( RenderableEntity * renderable, model::IConnectedComponentPtr cc );
};


}	// bv