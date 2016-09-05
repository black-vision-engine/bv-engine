#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

#include "Engine/Models/Cameras/CameraModel.h"

namespace bv
{
class Camera;
class RenderableEntity;

class UpdatersHelpers
{
public:

    static void         WriteVertexDataToVBO        ( char * data, model::IConnectedComponentPtr cc );
    static void         UpdateRenderableBuffer      ( RenderableEntity * renderable, model::IConnectedComponentPtr cc );
    static void         RecreateRenderableBuffer    ( RenderableEntity * renderable, model::IConnectedComponentPtr cc );

    static void         UpdateCamera                ( Camera * camera, model::CameraModelPtr & cameraModel );
};


}	// bv