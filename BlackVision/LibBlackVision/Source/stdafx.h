#pragma once

//#define DISABLE_PRECOMPILED_HEADER

#ifndef DISABLE_PRECOMPILED_HEADER

#include "CoreDEF.h"
#include "Tools/Logger/Logger.h"

#include "Mathematics/glm_inc.h"
#include "Mathematics/AABB.h"
#include "Mathematics/Box.h"
#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Interpolators/CompositeInterpolator.h"

#include "Engine/Models/BoundingVolume.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializable.h"
#include "Serialization/DeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"
//#include "Serialization/BV/CloneViaSerialization.h"

#include "Serialization/SerializationHelper.h"


#include "Engine/Events/Events.h"
#include "Engine/Events/EventManager.h"


#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Simple/GeometryProcessors/DefaultGeometryProcessorBase.h"


//#include "Memory/MemoryLeaks.h"
//#include <vld.h>

#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"

#include "Assets/Asset.h"
#include "Assets/AssetDescriptor.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetManager.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/3D/FontAsset3DDesc.h"
#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Mesh/MeshAssetDescriptor.h"
#include "Assets/SVG/SVGAssetDescriptor.h"


#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimeEvaluatorBase.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


#include "Engine/Graphics/State/RendererStateInstance.h"
#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/NodeEffectLogic.h"
#include "Engine/Graphics/Shaders/RenderablePass.h"

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/TransformableEntity.h"

#include "UseLoggerLibBlackVision.h"



#include <map>
#include <string>
#include <vector>
#include <hash_map>
#include <memory>

#endif

#undef near
#undef far
#undef max
#undef min
#undef LoadImage
#undef LoadImageW

